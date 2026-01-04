#include "Physics.h"

#include "../app/Config.h"
#include "Level.h"
#include "entt/entity/fwd.hpp"
#include <cstdio>
#include <optional>

static const LevelTilemap *tiles = nullptr;
static const LevelTilemap *airflow = nullptr;

void setPhysicsColliderData(const LevelLayout &level) {
    tiles = &level.GetTiles();
    airflow = &level.GetAirflow();
}

int xPosToTileCoord(int x) {
    return x / UNITS_PER_BLOCK - 2;
}

int yPosToTileCoord(int y) {
    return y / UNITS_PER_BLOCK;
}

Vector2Int posToTileCoords(int x, int y) {
    return {xPosToTileCoord(x), yPosToTileCoord(y)};
}

bool collidesWithWall(entt::registry &registry, const Position &position, const Collider &collider) {
    Vector2Int pos = position.toVector();
    for (int x = xPosToTileCoord(collider.left(pos)); x <= xPosToTileCoord(collider.right(pos) - 1); x++) {
        for (int y = yPosToTileCoord(collider.top(pos)); y <= yPosToTileCoord(collider.bottem(pos) - 1); y++) {
            // collides with 2 space walls on level sides, but not with walls over or below the level
            if (x < 0 || x >= LevelTilemap::WIDTH || (y >= 0 && y < LevelTilemap::HEIGHT && !tiles->IsEmpty(x, y))) {
                return true;
            }
        }
    }
    return false;
}

template <typename ColliderType>
bool collidesWithCollider(entt::registry &registry, const Position &position, const Collider &collider) {
    const auto view = registry.view<Position, ColliderType>();

    for (const auto entity : view) {
        const auto [pos, col] = view.get(entity);

        if (overlaps(position, collider, pos, col)) {
            return true;
        }
    }

    return false;
}

template <typename ColliderType>
bool collidesWithMultiCollider(entt::registry &registry, const Position &position, const Collider &collider) {
    const auto view = registry.view<Position, ColliderType>();

    for (const auto entity : view) {
        auto [pos, multiCollider] = view.get(entity);

        for (Collider &col : multiCollider.colliders) {
            if (overlaps(position, collider, pos, col)) {
                return true;
            }
        }
    }

    return false;
}

bool collidesWithJumpableBubble(entt::registry &registry, const Position &position, const Collider &collider) {
    const auto view = registry.view<Position, BubbleFloatComponent>(entt::exclude<DestroyEntity>);

    for (const auto entity : view) {
        const auto [pos, bubble] = view.get(entity);

        if (overlaps(position, collider, pos, Colliders::bubbleJumpableCollider)) {
            return true;
        }
    }

    return false;
}

std::optional<entt::entity> getCollidingShootingBubble(entt::registry &registry, const Position &position, const Collider &collider) {
    const auto view = registry.view<Position, BubbleShootComponent>(entt::exclude<DestroyEntity>);

    for (const auto entity : view) {
        const auto [pos, bubble] = view.get(entity);

        if (!bubble.isWaiting() && overlaps(position, collider, pos, Colliders::bubbleCollider)) {
            return entity;
        }
    }

    return {};
}

bool collidesWithDragonSpikes(entt::registry &registry, const Position &position, const Collider &collider) {
    const auto view = registry.view<Position, DragonTag>();

    for (const auto entity : view) {
        auto [pos] = view.get(entity);

        DragonSpikeCollider multiCollider = Colliders::dragonSpikeCollider;
        if (pos.dir > 0) {
            multiCollider.flipX(BP_SIZE(2, 0));
        }

        for (Collider &col : multiCollider.colliders) {
            if (overlaps(position, collider, pos, col)) {
                return true;
            }
        }
    }

    return false;
}

bool collidesWithEnemy(entt::registry &registry, const Position &position, const Collider &collider) {
    const auto view = registry.view<Position, EnemyTag>(entt::exclude<DestroyEntity, BubbleFloatComponent, BubblePopComponent>);

    for (const auto entity : view) {
        const auto [pos] = view.get(entity);

        if (overlaps(position, collider, pos, Colliders::fullActorCollider)) {
            return true;
        }
    }

    return false;
}

// template bool collidesWithCollider<DragonSpikeCollider>(entt::registry& registry, const Position& position, const Collider& collider);
template bool collidesWithCollider<BubbleJumpableTopCollider>(entt::registry &registry, const Position &position, const Collider &collider);

template bool collidesWithMultiCollider<DragonSpikeCollider>(entt::registry &registry, const Position &position, const Collider &collider);

Vector2Int toDirection(LevelTileType type) {
    switch (type) {
    case LevelTileType::AIRFLOW_UP:
        return Vector2Int::Down(); // Up and down are reversed due to different coordinate systems
    case LevelTileType::AIRFLOW_DOWN:
        return Vector2Int::Up(); // Also reversed
    case LevelTileType::AIRFLOW_RIGHT:
        return Vector2Int::Right();
    case LevelTileType::AIRFLOW_LEFT:
        return Vector2Int::Left();
    default:
        return Vector2Int::Zero();
    }
}

Vector2Int getAirflowDirection(const Collider &col, const Vector2Int &pos) {
    Vector2Int tileCoords = posToTileCoords(pos.X, pos.Y);

    Vector2Int dir = Vector2Int::Zero();

    for (int x = xPosToTileCoord(col.left(pos)); x <= xPosToTileCoord(col.right(pos)); x++) {
        for (int y = yPosToTileCoord(col.top(pos)); y <= yPosToTileCoord(col.bottem(pos)); y++) {
            dir += toDirection(airflow->Get(x, y, true));
        }
    }

    dir.IntegerNormalize();
    return dir;
}

int calculateMovementToRoundedPosition(const Position &pos, const Collider &col, int dir) {
    int coord = pos.x + col.offsetX + (dir > 0 ? col.width : 0);
    if (coord % UNITS_PER_BLOCK == 0) {
        return 0;
    }
    if (dir > 0) {
        return UNITS_PER_BLOCK - (coord % UNITS_PER_BLOCK);
    } else {
        return -(coord % UNITS_PER_BLOCK);
    }
}

template <typename Collider1, typename Collider2>
bool overlaps(const Position &pos1, const Collider1 &col1, const Position &pos2, const Collider2 &col2) {
    int xmin1 = pos1.x + col1.offsetX;
    int xmax1 = xmin1 + col1.width;
    int ymin1 = pos1.y + col1.offsetY;
    int ymax1 = ymin1 + col1.height;
    int xmin2 = pos2.x + col2.offsetX;
    int xmax2 = xmin2 + col2.width;
    int ymin2 = pos2.y + col2.offsetY;
    int ymax2 = ymin2 + col2.height;

    return xmax1 > xmin2 && xmax2 > xmin1 &&
           ymax1 > ymin2 && ymax2 > ymin1;
}
