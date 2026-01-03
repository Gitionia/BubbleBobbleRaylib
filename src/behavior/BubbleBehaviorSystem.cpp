#include "BubbleBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../utils/Debug.h"
#include "../utils/Log.h"
#include "Physics.h"
#include "entt/entt.hpp"

void BubbleBehaviorSystem::makeBubbleFloating(entt::entity entity) {
    entitiesToMakeFloating.push_back(entity);
}

void BubbleBehaviorSystem::Init() {
    // Probably a lower reserve would suffice, but this should be good enough without much cost
    entitiesToMakeFloating.reserve(20);
}

void BubbleBehaviorSystem::Update() {
    const Collider &col = Colliders::bubbleCollider;

    {
        auto view = registry.view<Position, BubbleShootComponent, RenderData>();
        for (auto entity : view) {
            auto [pos, bubble, renderData] = view.get(entity);

            int shootVelocity = UNITS_PER_BLOCK / 8;

            if (bubble.animator.IsFinished())
                bubble.animator.Reset();
            renderData.spriteHandle = bubble.animator.GetSpriteHandle();
            bubble.animator.Update();

            if (!bubble.isWaiting()) {
                int dx = shootVelocity * bubble.shootDirection;
                pos.x += dx;

                bubble.shootFrame--;
                if (bubble.shootFrame == 0) {
                    makeBubbleFloating(entity);
                }
                // if hits wall, enable jumpable delay and popable delay
                else if (collidesWithWall(registry, pos, col)) {
                    // Bug: Now Bubbles also get rounded if they get spawned in a wall
                    pos.x -= dx;
                    pos.x += calculateMovementToRoundedPosition(pos, col, bubble.shootDirection);

                    bubble.shootFrame = 0;
                    bubble.jumpableDelayFrame = bubble.JUMPABLE_DELAY_FRAME_COUNT;
                    bubble.popableDelayFrame = bubble.POPABLE_DELAY_FRAME_COUNT;
                }
            } else {
                if (bubble.popableDelayFrame > 0) {
                    bubble.popableDelayFrame--;
                }
                if (bubble.popableDelayFrame == 0 && collidesWithDragonSpikes(registry, pos, col)) {
                    Destroy(entity);
                }

                if (bubble.jumpableDelayFrame > 0) {
                    bubble.jumpableDelayFrame--;
                }
                if (bubble.jumpableDelayFrame == 0) {
                    makeBubbleFloating(entity);
                }
            }
        }
    }

    for (auto entity : entitiesToMakeFloating) {
        registry.remove<BubbleShootComponent>(entity);
        registry.emplace<BubbleFloatComponent>(entity);
    }
    entitiesToMakeFloating.clear();

    {
        auto view = registry.view<Position, BubbleFloatComponent, RenderData>();
        for (auto entity : view) {
            auto [pos, bubble, renderData] = view.get(entity);

            if (bubble.animator.IsFinished())
                bubble.animator.Reset();
            renderData.spriteHandle = bubble.animator.GetSpriteHandle();
            bubble.animator.Update();

            Vector2Int centerPos = col.getCenter(pos.x, pos.y);
            Vector2Int airflowVelocity = getAirflowDirection(col, pos.toVector());

            pos.x += airflowVelocity.X;
            if (collidesWithWall(registry, pos, col)) {
                pos.x -= airflowVelocity.X;
            }

            pos.y += airflowVelocity.Y;
            if (collidesWithWall(registry, pos, col)) {
                pos.y -= airflowVelocity.Y;
            }

            if (pos.y >= BP_SIZE(LevelTilemap::HEIGHT + 1, 2)) {
                pos.y = BP_SIZE(-2, 0);
            } else if (pos.y < BP_SIZE(-2, -2)) {
                pos.y = BP_SIZE(LevelTilemap::HEIGHT, -2);
            }

            // Debug::DrawPoint(centerPos.X + airflowVelocity.X, centerPos.Y +
            // airflowVelocity.Y, 32, { 0, 122, 122, 180});

            if (collidesWithDragonSpikes(registry, pos, col)) {
                Destroy(entity);
            }

            if (bubble.lifetimeFrame > 0) {
                bubble.lifetimeFrame--;
            } else {
                Destroy(entity);
            }
        }
    }
}
