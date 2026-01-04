#include "DragonBehaviorSystem.h"

#include <iostream>
#include <iterator>

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../utils/Debug.h"
#include "../utils/Input.h"
#include "Level.h"
#include "Physics.h"
#include "WalkingActorUtils.h"
#include "entt/entity/fwd.hpp"

void makeDragonHit(entt::registry &registry, entt::entity e) {
    registry.emplace<DragonHitComponent>(e);
}

void DragonBehaviorSystem::Init() {
}

void DragonBehaviorSystem::Update() {

    static Animator animator(&GetAnimation("Dragon-Idle"));

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, WalkingActorComponent, DragonComponent, RenderData>(entt::exclude<DragonHitComponent>);
    for (auto entity : view) {
        auto [pos, actor, dragon, renderData] = view.get(entity);

        if (collidesWithEnemy(registry, pos, Colliders::fullActorCollider)) {
            Defer(entity, &makeDragonHit, 0);
            continue;
        }

        // animations and sprite
        animator.Update();
        if (animator.IsFinished()) {
            if (dragon.state == DragonComponent::SHOOTING) {
                animator.SetNewAnimation(&GetAnimation("Dragon-Idle"));
                dragon.state = DragonComponent::IDLE;

            } else if (dragon.state == DragonComponent::WALKING) {
                animator.Reset();
            } else if (dragon.state == DragonComponent::IDLE) {
                animator.Reset();
            } else if (dragon.state == DragonComponent::JUMPING || dragon.state == DragonComponent::FALLING) {
                animator.Reset();
            }
        }
        renderData.spriteHandle = animator.GetSpriteHandle();

        int JUMP_FRAME_COUNT = 5 * (int)(UNITS_PER_BLOCK * 1.2f) / actor.jumpSpeed;

        int inputDir = Input::GetXAxis();
        bool jump = Input::IsKeyDown(Key::Jump);

        if (inputDir != 0) {
            pos.dir = inputDir;

            if (dragon.state == DragonComponent::IDLE) {
                animator.SetNewAnimation(&GetAnimation("Dragon-Walking"));
                dragon.state = DragonComponent::WALKING;
            }
        } else {
            if (dragon.state == DragonComponent::WALKING) {
                animator.SetNewAnimation(&GetAnimation("Dragon-Idle"));
                dragon.state = DragonComponent::IDLE;
            }
        }

        int moveSpeed = UNITS_PER_BLOCK / 16;
        int velx = moveSpeed * inputDir;

        // shooting
        if (dragon.bubbleShootDelay == 0 && Input::IsKeyDown(Key::Fire)) {
            EntityFactory::CreateBubbleCenteredAt(pos.toVector().Add(BP_SIZE(1, 0), BP_SIZE(1, 0)), pos.dir);
            dragon.bubbleShootDelay = dragon.MAX_BUBBLE_SHOOT_DELAY;

            animator.SetNewAnimation(&GetAnimation("Dragon-Shooting"));
            dragon.state = DragonComponent::SHOOTING;
        } else if (dragon.bubbleShootDelay > 0) {
            dragon.bubbleShootDelay--;
        }

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, actor);

        // walking
        pos.x += velx;
        if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
            pos.x -= velx;
        }

        // check if grounded
        bool isGrounded = false;
        if (!actor.isJumping()) {
            isGrounded = isWalkingActorGrounded(registry, pos, actor);
        }

        if (!actor.isJumping()) {
            if (!isGrounded && dragon.state != DragonComponent::FALLING && dragon.state != DragonComponent::SHOOTING) {
                animator.SetNewAnimation(&GetAnimation("Dragon-Falling"));
                dragon.state = DragonComponent::FALLING;

            } else if (isGrounded) {
                if (inputDir == 0) {
                    animator.SetNewAnimation(&GetAnimation("Dragon-Idle"));
                    dragon.state = DragonComponent::IDLE;
                } else {
                    animator.SetNewAnimation(&GetAnimation("Dragon-Walking"));
                    dragon.state = DragonComponent::WALKING;
                }
            }

        } else {
            if (dragon.state != DragonComponent::JUMPING) {
                animator.SetNewAnimation(&GetAnimation("Dragon-Jumping"));
                dragon.state = DragonComponent::JUMPING;
            }
        }

        // start jump
        if (jump && !actor.isJumping()) {
            if (isGrounded || collidesWithJumpableBubble(registry, pos, collider)) {
                dragon.jumpSpeed.reset();
                actor.jumpFrameCount = dragon.jumpSpeed.getLength();
            }
        }

        if (actor.isJumping()) {
            actor.jumpSpeed = dragon.jumpSpeed.get();
            dragon.jumpSpeed.tick();
        }
    }
}
