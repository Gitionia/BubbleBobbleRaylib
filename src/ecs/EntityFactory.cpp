#include "EntityFactory.h"

#include "Components.h"
#include "entt/entt.hpp"

entt::entity EntityFactory::CreateTile(int x, int y, Color colorShadowRight, Color colorShadowBottem, bool addShadowRight, bool addShadowBottem) {
    entt::registry *registry = get().registry;
    auto tile = registry->create();
    {
        registry->emplace<Position>(tile, x * UNITS_PER_BLOCK, y * UNITS_PER_BLOCK);
        RenderData data = {GetSpriteHandle("Level1"), {1, 1}};
        registry->emplace<RenderData>(tile, data);
        registry->emplace<LevelTileTag>(tile);
    }

    if (addShadowRight) {
        auto shadowRight = registry->create();
        registry->emplace<Position>(shadowRight, (x + 1) * UNITS_PER_BLOCK, y * UNITS_PER_BLOCK);
        RenderData data = {GetSpriteHandle("TileShadowRight"), {2, 2}};
        data.SetColor(colorShadowRight);
        registry->emplace<RenderData>(shadowRight, data);
        registry->emplace<LevelTileShadowTag>(shadowRight);
    }
    if (addShadowBottem) {
        auto shadowBottem = registry->create();
        registry->emplace<Position>(shadowBottem, x * UNITS_PER_BLOCK, (y + 1) * UNITS_PER_BLOCK);
        RenderData data = {GetSpriteHandle("TileShadowBottem"), {2, 2}};
        data.SetColor(colorShadowBottem);
        registry->emplace<RenderData>(shadowBottem, data);
        registry->emplace<LevelTileShadowTag>(shadowBottem);
    }

    return tile;
}

entt::entity EntityFactory::CreateDragon() {
    entt::registry *registry = get().registry;
    auto dragon = registry->create();

    registry->emplace<Position>(dragon, 5 * UNITS_PER_BLOCK, 5 * UNITS_PER_BLOCK);
    registry->emplace<RenderData>(dragon, RenderData(GetSpriteHandle("Dragon-Idle-1"), {2, 2}));
    registry->emplace<WalkingActorComponent>(dragon, UNITS_PER_BLOCK / 8, 3 * UNITS_PER_BLOCK / 16);
    registry->emplace<DragonComponent>(dragon);
    registry->emplace<DragonTag>(dragon);

    return dragon;
}

entt::entity EntityFactory::CreateBubbleCenteredAt(const Vector2Int &centre, int direction) {
    entt::registry *registry = get().registry;
    auto bubble = registry->create();

    registry->emplace<Position>(bubble, centre.X - BP_SIZE(0, 14), centre.Y - BP_SIZE(1, 0));
    registry->emplace<RenderData>(bubble, RenderData(GetSpriteHandle("Bubble-Green-Idle-1"), {2, 2}));
    registry->emplace<BubbleComponent>(bubble, direction, Animator(&GetAnimation("Bubble-Green-Idle")));
    registry->emplace<BubbleTag>(bubble);

    return bubble;
}

void EntityFactory::MakeBubbleJumpable(entt::entity entity) {
    entt::registry *registry = get().registry;

    // Collider offset 2 pixels up to avoid poping the bubble when jumping
    registry->emplace<BubbleJumpableTopCollider>(entity, BP_SIZE(0, 28), BP_SIZE(0, 4), 0, BP_SIZE(0, -2));
}

entt::entity EntityFactory::CreateEnemy(int x, int y) {
    entt::registry *registry = get().registry;
    auto enemy = registry->create();

    registry->emplace<Position>(enemy, BP_SIZE(x + 2, 0), BP_SIZE(y, 0));
    registry->emplace<RenderData>(enemy, RenderData(GetSpriteHandle("Can-Walk-1"), {2, 2}));
    registry->emplace<EnemyComponent>(enemy, 0);
    registry->emplace<WalkingActorComponent>(enemy, UNITS_PER_BLOCK / 16, 3 * UNITS_PER_BLOCK / 16);
    registry->emplace<EnemyTag>(enemy);

    return enemy;
}

void EntityFactory::CreateLevel(const LevelLayout &level) {
    for (int x = 2; x < LevelTilemap::WIDTH + 2; ++x) {
        for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
            bool addShadowRight = LevelTilemap::OutOfRange((x - 2) + 1, y) ? true : level.GetTiles().IsEmpty((x - 2) + 1, y);
            bool addShadowBottem = LevelTilemap::OutOfRange((x - 2), y + 1) ? true : level.GetTiles().IsEmpty((x - 2), y + 1);
            if (!level.GetTiles().IsEmpty(x - 2, y)) {
                CreateTile(x, y, level.GetShadeColorRight(), level.GetShadeColorBottem(), addShadowRight, addShadowBottem);
            }
        }
    }

    int rows[] = {0, 1, 30, 31};
    for (int x : rows) {
        for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
            bool addShadowRight = x == 1;
            bool addShadowBottem = false;
            CreateTile(x, y, level.GetShadeColorRight(), level.GetShadeColorBottem(), addShadowRight, addShadowBottem);
        }
    }

    for (int x = 0; x < LevelTilemap::WIDTH; ++x) {
        for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
            if (!level.GetEnemies().IsEmpty(x, y)) {
                CreateEnemy(x, y);
            }
        }
    }
}

EntityFactory &EntityFactory::get() {
    static EntityFactory factory;
    return factory;
}

void EntityFactory::setRegistry(entt::registry &registry) {
    this->registry = &registry;
}
