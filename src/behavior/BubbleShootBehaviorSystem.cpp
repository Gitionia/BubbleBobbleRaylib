#include "BubbleShootBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../utils/Debug.h"
#include "../utils/Log.h"
#include "Physics.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"

enum DeferValues {
    Floating = 0,
    Pop = 1
};

static void makeBubbleFloating(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleShootComponent>(entity);
    registry.emplace<BubbleFloatComponent>(entity);
}

static void makeBubblePop(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleShootComponent>(entity);
    registry.emplace<BubblePopComponent>(entity);
}

void BubbleShootBehaviorSystem::Update() {
    const Collider &col = Colliders::bubbleCollider;

    auto view = registry.view<Position, BubbleShootComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, bubble, renderData] = view.get(entity);

        int shootVelocity = BP_SIZE(0, 4);
        int POPABLE_DELAY_FRAME_COUNT = 3;
        int JUMPABLE_DELAY_FRAME_COUNT = 5;

        renderData.spriteHandle = bubble.animator.GetSpriteHandle();
        bubble.animator.Update();

        if (!bubble.isWaiting()) {
            int dx = shootVelocity * bubble.shootDirection;
            pos.x += dx;

            bubble.shootFrame--;
            if (bubble.shootFrame == 0) {

                Defer(entity, &makeBubbleFloating, Floating);
            }
            // if hits wall, enable jumpable delay and popable delay
            else if (collidesWithWall(registry, pos, col)) {
                // Bug: Now Bubbles also get rounded if they get spawned in a wall
                pos.x -= dx;
                pos.x += calculateMovementToRoundedPosition(pos, col, bubble.shootDirection);

                bubble.shootFrame = 0;
                bubble.jumpableDelayFrame = JUMPABLE_DELAY_FRAME_COUNT;
                bubble.popableDelayFrame = POPABLE_DELAY_FRAME_COUNT;
            }
        } else {
            if (bubble.popableDelayFrame > 0) {
                bubble.popableDelayFrame--;
            }
            if (bubble.popableDelayFrame == 0 && collidesWithDragonSpikes(registry, pos, col)) {
                Defer(entity, &makeBubblePop, Pop);
            }

            if (bubble.jumpableDelayFrame > 0) {
                bubble.jumpableDelayFrame--;
            }
            if (bubble.jumpableDelayFrame == 0) {

                Defer(entity, &makeBubbleFloating, Floating);
            }
        }
    }
}
