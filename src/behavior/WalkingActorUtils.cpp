#include "WalkingActorUtils.h"

bool shouldWalkingActorIgnoreCollisions(entt::registry &registry, const Position &pos, const Collider &collider) {
    // Above and below the level the actor should ignore collisions.
    // Above includes all y-positions where the actor would be standing on the
    // top of the level or above that.
    if (pos.y > 24 * UNITS_PER_BLOCK || pos.y <= -2 * UNITS_PER_BLOCK) {
        return true;
    } else {
        return collidesWithWall(registry, pos, collider);
    }
}

bool isWalkingActorGrounded(entt::registry &registry, Position pos, const WalkingActorComponent &actor) {
    pos.y += actor.fallSpeed;
    bool isGrounded = !actor.ignoreCollisions && collidesWithWall(registry, pos, Colliders::walkingActorCollider);
    pos.y -= actor.fallSpeed;

    return isGrounded;
}

bool shouldWalkingEnemyGapJump(const Position &pos, int dir) {
    DBG_CHECK(dir == 1 || dir == -1, "Direction for checking for gap jump should be 1 or -1");

    int blockX;
    if (dir == -1) {

        blockX = xPosToTileCoord(pos.x);
        if (pos.x % BP_SIZE(1, 0) == 0) {
            blockX--;
        }

    } else {
        blockX = xPosToTileCoord(pos.x + BP_SIZE(2, 0));
    }

    int blockY = yPosToTileCoord(pos.y + BP_SIZE(2, 0));

    return !collisionAt(blockX, blockY) && !collisionAt(blockX + dir, blockY) && !collisionAt(blockX, blockY - 1) && !collisionAt(blockX + dir, blockY - 1) && !collisionAt(blockX + 2 * dir, blockY - 1);
}