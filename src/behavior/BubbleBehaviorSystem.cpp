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

        int shootVelocity = UNITS_PER_BLOCK / 16;

        if (bubble.state == BubbleState::SHOOTING) {
            int dx = shootVelocity * bubble.shootDirection;
            pos.x += dx;

            if (collidesWithWall(registry, pos, col)) {
                bubble.state = BubbleState::FLOATING;
                pos.x -= dx;
            }

            bubble.shootCounter--;
            if (bubble.shootCounter == 0) {
                bubble.state = BubbleState::FLOATING;
            }
        }
    }
}