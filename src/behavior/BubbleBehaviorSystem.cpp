#include "BubbleBehaviorSystem.h"

#include "../ecs/Components.h"
#include "entt/entt.hpp"
#include "../app/Config.h"
#include "Physics.h"


void BubbleBehaviorSystem::Update()
{
    auto view = registry.view<Position, BubbleComponent, Collider>();
    for (auto entity : view) {
        auto [pos, bubble, col] = view.get(entity);

        int shootVelocity = UNITS_PER_BLOCK / 8;

        if (bubble.state == BubbleState::SHOOTING) {
            int dx = shootVelocity * bubble.shootDirection;
            
            pos.x += dx;

            

            if (collidesWithWall(registry, pos, col)) {
                bubble.state = BubbleState::FLOATING;
                registry.emplace<BubbleJumpableTopCollider>(entity, 2 * (UNITS_PER_BLOCK / 16) * 14, 4 * UNITS_PER_PIXEL, 0, 0);


                // Bug: Now Bubbles also get rounded if they get spawned in a wall
                pos.x -= dx;
                pos.x += calculateMovementToRoundedPosition(pos, col, bubble.shootDirection);
            }

            bubble.shootCounter--;
            if (bubble.shootCounter == 0) {
                bubble.state = BubbleState::FLOATING;
                registry.emplace<BubbleJumpableTopCollider>(entity, 2 * (UNITS_PER_BLOCK / 16) * 14, 4 * UNITS_PER_PIXEL, 0, 0);
            }
        }
        else if (bubble.state == BubbleState::FLOATING) {
            
            if (collidesWithMultiCollider<DragonSpikeCollider>(registry, pos, col)) {
                Destroy(entity);
            }

            bubble.lifetimeCounter--;
            if (bubble.lifetimeCounter == 0) {
                Destroy(entity);
            }
        }
    }
}