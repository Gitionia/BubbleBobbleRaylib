#include "BubbleBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../utils/Debug.h"
#include "Physics.h"
#include "entt/entt.hpp"

void BubbleBehaviorSystem::Update() {
    auto view = registry.view<Position, BubbleComponent, Collider, RenderData>();
    for (auto entity : view) {
        auto [pos, bubble, col, renderData] = view.get(entity);

        int shootVelocity = UNITS_PER_BLOCK / 8;

        if (bubble.animator.IsFinished())
            bubble.animator.Reset();
        renderData.spriteHandle = bubble.animator.GetSpriteHandle();
        bubble.animator.Update();

        switch (bubble.state) {
        case BubbleState::SHOOTING: {
            int dx = shootVelocity * bubble.shootDirection;

            pos.x += dx;

            if (collidesWithWall(registry, pos, col)) {
                // Bug: Now Bubbles also get rounded if they get spawned in a wall
                pos.x -= dx;
                pos.x += calculateMovementToRoundedPosition(pos, col, bubble.shootDirection);

                bubble.state = BubbleState::FLOATING;
                bubble.shootCounter = 0;

                registry.emplace<BubbleJumpableTopCollider>(entity, 2 * (UNITS_PER_BLOCK / 16) * 14,
                                                            4 * UNITS_PER_PIXEL, 0, 0);

            } else {
                bubble.shootCounter--;
                if (bubble.shootCounter == 0) {
                    bubble.state = BubbleState::FLOATING;
                    registry.emplace<BubbleJumpableTopCollider>(entity, 2 * (UNITS_PER_BLOCK / 16) * 14,
                                                                4 * UNITS_PER_PIXEL, 0, 0);
                }
            }

            break;
        }
        case BubbleState::FLOATING: {
            Vector2Int centerPos = col.getCenter(pos.x, pos.y);
            Vector2Int airflowVelocity = getAirflowDirection(pos.x, pos.y);

            pos.x += airflowVelocity.X;
            if (collidesWithWall(registry, pos, col)) {
                pos.x -= airflowVelocity.X;
            }

            pos.y += airflowVelocity.Y;
            if (collidesWithWall(registry, pos, col)) {
                pos.y -= airflowVelocity.Y;
            }

            // Debug::DrawPoint(centerPos.X + airflowVelocity.X, centerPos.Y +
            // airflowVelocity.Y, 32, { 0, 122, 122, 180});

            if (collidesWithMultiCollider<DragonSpikeCollider>(registry, pos, col)) {
                Destroy(entity);
            }

            bubble.lifetimeCounter--;
            if (bubble.lifetimeCounter == 0) {
                Destroy(entity);
            }

            break;
        }

        default:
            break;
        }
    }
}
