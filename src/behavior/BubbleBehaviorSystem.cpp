#include "BubbleBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../utils/Debug.h"
#include "../utils/Log.h"
#include "Physics.h"
#include "entt/entt.hpp"

void BubbleBehaviorSystem::Update() {
    static Collider col { BP_SIZE(0, 28), BP_SIZE(2, 0), 0, 0 };

    auto view = registry.view<Position, BubbleComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, bubble, renderData] = view.get(entity);

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
                bubble.jumpableDelay = bubble.MAX_JUMPABLE_DELAY;
                bubble.popableDelay = bubble.MAX_POPABLE_DELAY;

            } else {
                bubble.shootCounter--;
                if (bubble.shootCounter == 0) {
                    bubble.state = BubbleState::FLOATING;
                    EntityFactory::MakeBubbleJumpable(entity);
                }
            }

            break;
        }
        case BubbleState::FLOATING: {

            if (bubble.jumpableDelay > 0) {
                bubble.jumpableDelay--;
                if (bubble.jumpableDelay == 0) {
                    EntityFactory::MakeBubbleJumpable(entity);
                }
            }

            if (bubble.popableDelay > 0) {
                bubble.popableDelay--;
            }

            if (bubble.jumpableDelay == 0) {
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
            }

            if (pos.y >= BP_SIZE(LevelTilemap::HEIGHT + 1, 2)) {
                pos.y = BP_SIZE(-2, 0);
            } else if (pos.y < BP_SIZE(-2, -2)) {
                pos.y = BP_SIZE(LevelTilemap::HEIGHT, -2);
            }

            // Debug::DrawPoint(centerPos.X + airflowVelocity.X, centerPos.Y +
            // airflowVelocity.Y, 32, { 0, 122, 122, 180});

            if (bubble.popableDelay == 0 && collidesWithMultiCollider<DragonSpikeCollider>(registry, pos, col)) {
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
