#include "WalkingActorBehaviorSystem.h"

#include <iostream>

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../utils/Debug.h"
#include "../utils/Input.h"
#include "Level.h"
#include "Physics.h"
#include "WalkingActorUtils.h"

void WalkingActorBehaviorSystem::Init() {
}

void WalkingActorBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, WalkingActorComponent>();
    for (auto entity : view) {
        auto [pos, actor] = view.get(entity);

        int vely = 0;
        int BOTTEM_WARP_POS = 27 * UNITS_PER_BLOCK;
        // int TOP_WARP_POS = 30 * UNITS_PER_BLOCK;
        

        // clamp x position to inside of the level
        pos.x = std::max(2 * UNITS_PER_BLOCK, pos.x);
        pos.x = std::min(28 * UNITS_PER_BLOCK, pos.x);

        // execute jump
        if (actor.isJumping) {
            actor.jumpFrameCount--;

            if (actor.jumpFrameCount == 0) {
                actor.isJumping = false;
            }
        }

        if (actor.isJumping) {
            vely = -actor.jumpSpeed;
        } else {
            vely = actor.fallSpeed;
        }

        // execute falling
        pos.y += vely;
        if (!actor.isJumping) {
            if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
                DBG_ASSERT(vely > 0);
                pos.y = (pos.y / UNITS_PER_BLOCK) * UNITS_PER_BLOCK;
            }
        }

        // apply warping
        if (pos.y >= BOTTEM_WARP_POS) {
            pos.y = BP_SIZE(-3, 0);
        } else if (pos.y < BP_SIZE(-4, 0)) {
            pos.y = BP_SIZE(LevelTilemap::HEIGHT + 1, 0);
        }
    }
}
