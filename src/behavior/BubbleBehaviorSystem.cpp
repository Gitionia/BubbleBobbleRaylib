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

                // Bug: Now Bubbles also get rounded if they get spawned in a wall
                pos.x -= dx;
                pos.x += calculateMovementToRoundedPosition(pos, col, bubble.shootDirection);
            }

            bubble.shootCounter--;
            if (bubble.shootCounter == 0) {
                bubble.state = BubbleState::FLOATING;
            }
        }
        else if (bubble.state == BubbleState::FLOATING) {
            
            if (collidesWith<DragonSpikeCollider>(registry, pos, col)) {
                registry.destroy(entity);
            }

            bubble.lifetimeCounter--;
            if (bubble.lifetimeCounter == 0) {
                registry.destroy(entity);
            }
        }
    }
}