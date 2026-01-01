#include "TrashCanBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../utils/Debug.h"
#include "../utils/Input.h"
#include "../utils/Random.h"
#include "Level.h"
#include "Physics.h"
#include "WalkingActorUtils.h"

void TrashCanBehaviorSystem::Init() {
}

void TrashCanBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    static Animator animator(&GetAnimation("Can-Walk"));

    auto view = registry.view<Position, WalkingActorComponent, EnemyComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, actor, enemy, renderData] = view.get(entity);

        animator.Update();
        if (animator.IsFinished()) {
            animator.Reset();
        }
        renderData.spriteHandle = animator.GetSpriteHandle();

        int velx = 0;
        int vely = 0;
        int moveSpeed = UNITS_PER_BLOCK / 16;
        int jumpSpeed = 3 * UNITS_PER_BLOCK / 16;
        int fallSpeed = UNITS_PER_BLOCK / 16;

        int BOTTEM_WARP_POS = 27 * UNITS_PER_BLOCK;
        int TOP_WARP_POS = 30 * UNITS_PER_BLOCK;

        int JUMP_FRAME_COUNT = BP_SIZE(5, 0) / jumpSpeed;

        bool jump = false;

        // check if grounded
        bool isGrounded = false;
        if (!actor.isJumping) {
            isGrounded = isWalkingActorGrounded(registry, pos, actor);
        }

        if (isGrounded) {
            if (enemy.walkingDir == 0) {
                enemy.walkingDir = Random::Get().GetDirection();
            }

            pos.dir = enemy.walkingDir;
        } else {
            enemy.walkingDir = 0;
        }

        velx = moveSpeed * enemy.walkingDir;

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, actor);

        // start jump
        if (jump) {
            if (isGrounded) {
                actor.isJumping = true;
                actor.jumpFrameCount = JUMP_FRAME_COUNT;
            }
        }

        pos.x += velx;
        if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {

            pos.x -= velx;
            enemy.walkingDir *= -1;

            // check if enemy is in 2 space gap, in that case don't flip direction
            pos.x -= velx;
            if (collidesWithWall(registry, pos, collider)) {
                enemy.walkingDir *= -1;
            }
            pos.x += velx;
        }
        
    }
}
