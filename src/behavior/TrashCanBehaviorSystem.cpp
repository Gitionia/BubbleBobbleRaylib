#include "TrashCanBehaviorSystem.h"


#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../utils/Debug.h"
#include "../utils/Input.h"
#include "../utils/Random.h"
#include "Level.h"
#include "Physics.h"

void TrashCanBehaviorSystem::Init() {
}

void TrashCanBehaviorSystem::Update() {

    static Animator animator(&GetAnimation("Can-Walk"));
    
    auto view = registry.view<Position, WalkingActorComponent, EnemyComponent, RenderData, Collider>();
    for (auto entity : view) {
        auto [pos, actor, enemy, renderData, collider] = view.get(entity);

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

            pos.y += fallSpeed;
            if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
                isGrounded = true;
            }
            pos.y -= fallSpeed;
        }


        if (isGrounded) {
            if (enemy.walkingDir == 0) {
                enemy.walkingDir = Random::Get().GetDirection();
            }
        } else {
            enemy.walkingDir = 0;
        }

        velx = moveSpeed * enemy.walkingDir;

        bool targetFlip;
        if (velx > 0) {
            targetFlip = true;
        } else if (velx < 0) {
            targetFlip = false;
        }

        if (velx != 0) {
            if (targetFlip != renderData.flipX) {
                // dragonSpikes.flipX(2 * UNITS_PER_BLOCK);
            }
            renderData.flipX = targetFlip;
        }

        
        // Above and below the level the dragon should ignore collisions.
        // Above includes all y-positions where the dragon would be standing on the
        // top of the level or above that.
        if (pos.y > 24 * UNITS_PER_BLOCK || pos.y <= -2 * UNITS_PER_BLOCK) {
            actor.ignoreCollisions = true;
        } else {
            actor.ignoreCollisions = collidesWithWall(registry, pos, collider);
        }

        // start jump
        if (jump) {
            if (isGrounded) {
                actor.isJumping = true;
                actor.jumpFrameCount = 0;
            }
        }

        // execute jump
        if (actor.isJumping) {
            actor.jumpFrameCount++;
            // vely = -2 * dragonJumpingSpeeds[actor.jumpFrameCount];
            vely = -jumpSpeed;

            if (actor.jumpFrameCount == JUMP_FRAME_COUNT - 1) {
                actor.isJumping = false;
            }
        } else {
            vely = fallSpeed;
        }

        pos.x += velx;
        if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
            pos.x -= velx;

            enemy.walkingDir *= -1;
        }
        pos.x = std::max(2 * UNITS_PER_BLOCK, pos.x);
        pos.x = std::min(28 * UNITS_PER_BLOCK, pos.x);

        pos.y += vely;
        if (!actor.isJumping) {
            if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
                DBG_ASSERT(vely > 0);
                pos.y = (pos.y / UNITS_PER_BLOCK) * UNITS_PER_BLOCK;
            }
        }

        if (pos.y >= BOTTEM_WARP_POS) {
            pos.y = BP_SIZE(-3, 0);
        } else if (pos.y < BP_SIZE(-4, 0)) {
            pos.y = BP_SIZE(LevelTilemap::HEIGHT + 1, 0);
        }
    }
}
