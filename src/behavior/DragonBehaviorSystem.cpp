#include "DragonBehaviorSystem.h"

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

void DragonBehaviorSystem::Init() {
}

void DragonBehaviorSystem::Update() {

    static Animator animator(&GetAnimation("Dragon-Idle"));

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, WalkingActorComponent, DragonComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, actor, dragon, renderData] = view.get(entity);

        // animations and sprite
        animator.Update();
        if (animator.IsFinished()) {
            if (animator.GetAnimationName() == "Dragon-Shooting") {
                animator.SetNewAnimation(&GetAnimation("Dragon-Idle"));
            }
            animator.Reset();
        }
        renderData.spriteHandle = animator.GetSpriteHandle();

        int JUMP_FRAME_COUNT = 5 * (int)(UNITS_PER_BLOCK * 1.2f) / actor.jumpSpeed;
        
        int moveSpeed = UNITS_PER_BLOCK / 16;
        int velx = moveSpeed * Input::GetXAxis();
        bool jump = Input::IsKeyDown(Key::Jump);

        if (velx != 0) {
            pos.dir = velx < 0 ? -1 : 1;
        }

        if (dragon.bubbleShootDelay == 0 && Input::IsKeyDown(Key::Fire)) {
            EntityFactory::CreateBubbleCenteredAt(pos.toVector().Add(BP_SIZE(1, 0), BP_SIZE(1, 0)), pos.dir);
            dragon.bubbleShootDelay = dragon.MAX_BUBBLE_SHOOT_DELAY;

            animator.SetNewAnimation(&GetAnimation("Dragon-Shooting"));
        } else if (dragon.bubbleShootDelay > 0) {
            dragon.bubbleShootDelay--;
        }

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, actor);

        pos.x += velx;
        if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
            pos.x -= velx;
        }

        // check if grounded
        bool isGrounded;
        if (!actor.isJumping) {
            isGrounded = isWalkingActorGrounded(registry, pos, actor);
        }

        // start jump
        if (jump) {
            if (isGrounded || collidesWithCollider<BubbleJumpableTopCollider>(registry, pos, collider)) {
                actor.isJumping = true;
                actor.jumpFrameCount = JUMP_FRAME_COUNT;
            }
        }
        
    }
}
