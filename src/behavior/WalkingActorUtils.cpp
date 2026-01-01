#include "WalkingActorUtils.h"

bool shouldWalkingActorIgnoreCollisions(entt::registry &registry, const Position &pos, const WalkingActorComponent &actor) {
    // Above and below the level the actor should ignore collisions.
    // Above includes all y-positions where the actor would be standing on the
    // top of the level or above that.
    if (pos.y > 24 * UNITS_PER_BLOCK || pos.y <= -2 * UNITS_PER_BLOCK) {
        return true;
    } else {
        return collidesWithWall(registry, pos, Colliders::walkingActorCollider);
    }
}

bool isWalkingActorGrounded(entt::registry &registry, Position pos, const WalkingActorComponent &actor) {
    pos.y += actor.fallSpeed;
    bool isGrounded = !actor.ignoreCollisions && collidesWithWall(registry, pos, Colliders::walkingActorCollider);
    pos.y -= actor.fallSpeed;

    return isGrounded;
}
