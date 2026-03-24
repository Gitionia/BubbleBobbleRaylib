#include "DragonBehaviorSystem.h"


#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "Level.h"
#include "Physics.h"
#include "WalkingActorUtils.h"

void makeDragonHit(entt::registry &registry, entt::entity e) {
    registry.emplace<DragonHitComponent>(e);
}

void DragonBehaviorSystem::Init() {
}

void DragonBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, WalkingActorComponent, DragonComponent, RenderData>(entt::exclude<DragonHitComponent>);
    for (auto entity : view) {
        auto [pos, actor, dragon, renderData] = view.get(entity);

        if (collidesWithEnemy(registry, pos, Colliders::fullActorCollider)) {
            Defer(entity, &makeDragonHit, 0);
            continue;
        }

        const int JUMP_FRAME_COUNT = 5 * (int)(UNITS_PER_BLOCK * 1.2f) / actor.jumpSpeed;

        int inputDir = Input::GetXAxis();
        bool jump = Input::IsKeyDown(Key::Jump);

        int moveSpeed = UNITS_PER_BLOCK / 16;
        int velx = moveSpeed * inputDir;

        bool startedShootingAnimation = false;

        if (inputDir != 0) {
            pos.dir = inputDir;
        }

        // shooting
        if (dragon.bubbleShootDelay == 0 && Input::IsKeyDown(Key::Fire)) {
            EntityFactory::CreateBubbleCenteredAt(pos.toVector().Add(BP_SIZE(1, 0), BP_SIZE(1, 0)), pos.dir);
            dragon.bubbleShootDelay = dragon.MAX_BUBBLE_SHOOT_DELAY;

            startedShootingAnimation = true;

        } else if (dragon.bubbleShootDelay > 0) {
            dragon.bubbleShootDelay--;
        }

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, actor);

        // walking
        pos.x += velx;
        if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
            pos.x -= velx;
        }

        // check if is grounded
        bool isGrounded = false;
        if (!actor.isJumping()) {
            isGrounded = isWalkingActorGrounded(registry, pos, actor);
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

        dragon.animator.Update();
        // If shooting and not finished we won't change the animation
        if (!(dragon.state == DragonComponent::SHOOTING && !dragon.animator.IsFinished())) {

            if (startedShootingAnimation) {

                dragon.animator.SetNewAnimation(&GetAnimation("Dragon-Shooting"));
                dragon.state = DragonComponent::SHOOTING;

            } else if (isGrounded) {

                if (inputDir == 0 && dragon.state != DragonComponent::IDLE) {
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-Idle"));
                    dragon.state = DragonComponent::IDLE;

                } else if (inputDir != 0 && dragon.state != DragonComponent::WALKING) {
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-Walking"));
                    dragon.state = DragonComponent::WALKING;
                }

            } else {

                if (actor.isJumping() && dragon.state != DragonComponent::JUMPING) {
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-Jumping"));
                    dragon.state = DragonComponent::JUMPING;

                } else if (!actor.isJumping() && dragon.state != DragonComponent::FALLING) {
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-Falling"));
                    dragon.state = DragonComponent::FALLING;
                }
            }
        }

        if (dragon.animator.IsFinished() && dragon.state != DragonComponent::SHOOTING) {
            dragon.animator.Reset();
        }

        renderData.spriteHandle = dragon.animator.GetSpriteHandle();
    }
}
