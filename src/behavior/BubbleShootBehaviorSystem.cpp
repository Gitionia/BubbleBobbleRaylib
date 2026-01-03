#include "BubbleShootBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../utils/Debug.h"
#include "../utils/Log.h"
#include "Physics.h"
#include "entt/entt.hpp"

void BubbleShootBehaviorSystem::makeBubbleFloating(entt::entity entity) {
    entitiesToMakeFloating.push_back(entity);
}

void BubbleShootBehaviorSystem::Init() {
    // Probably a lower reserve would suffice, but this should be good enough without much cost
    entitiesToMakeFloating.reserve(20);
}

void BubbleShootBehaviorSystem::Update() {
    const Collider &col = Colliders::bubbleCollider;

    auto view = registry.view<Position, BubbleShootComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, bubble, renderData] = view.get(entity);

        int shootVelocity = UNITS_PER_BLOCK / 8;

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

    for (auto entity : entitiesToMakeFloating) {
        registry.remove<BubbleShootComponent>(entity);
        registry.emplace<BubbleFloatComponent>(entity);
    }
    entitiesToMakeFloating.clear();
}
