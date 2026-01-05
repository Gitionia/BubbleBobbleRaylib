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
    Pop = 1,
    FloatingAndWaiting = 2
};

static void makeBubbleFloating(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleShootComponent>(entity);
    registry.emplace<BubbleFloatComponent>(entity);
}

static void makeBubblePop(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleShootComponent>(entity);
    registry.emplace<BubblePopComponent>(entity);
}

static void makeBubbleFloatingAndWaiting(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleShootComponent>(entity);
    BubbleFloatComponent &c = registry.emplace<BubbleFloatComponent>(entity);

    c.popFrame = 10;
}

bool wallGapExists(entt::registry &registry, const Position &pos, const Collider &col) {
    return false;
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

        if (bubble.state == BubbleShootComponent::NONE) {
            if (collidesWithWall(registry, pos, col)) {

                if (!wallGapExists(registry, pos, col)) {
                    bubble.state = BubbleShootComponent::IGNORE_COLLISION_WAIT;
                    bubble.ignoreCollision = true;

                    bubble.ignoreCollisionWaitFrame = 10;
                

                } else {
                    bubble.state = BubbleShootComponent::IGNORE_COLLISION_SHOOT;
                    bubble.ignoreCollision = true;
                }

            } else {
                bubble.state = BubbleShootComponent::NORMAL_SHOOT;
                bubble.ignoreCollision = false;
            }

        } else if (bubble.state == BubbleShootComponent::IGNORE_COLLISION_SHOOT && bubble.ignoreCollision) {
            bubble.ignoreCollision = collidesWithWall(registry, pos, col);
        }

        if (bubble.state == BubbleShootComponent::IGNORE_COLLISION_WAIT) {
            bubble.ignoreCollisionWaitFrame--;
            if (bubble.ignoreCollisionWaitFrame == 0) {
                Defer(entity, &makeBubbleFloatingAndWaiting, FloatingAndWaiting);
            }

        } else if (!bubble.isWaiting()) {
            int dx = shootVelocity * bubble.shootDirection;
            pos.x += dx;

            bubble.shootFrame--;
            if (bubble.shootFrame == 0) {

                if (collidesWithWall(registry, pos, col)) {
                    pos.x -= dx;
                    pos.x += calculateMovementToRoundedPosition(pos, col, bubble.shootDirection);
                }

                Defer(entity, &makeBubbleFloating, Floating);
            }
            // if hits wall (and no ignore collision shoot), enable jumpable delay and popable delay
            else if (!bubble.ignoreCollision && collidesWithWall(registry, pos, col)) {
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
                continue;
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
