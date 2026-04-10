#include "DragonBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../level/GameModifiers.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"
#include "raylib.h"

void makeDragonHit(entt::registry &registry, entt::entity e) {
    DragonColor color = registry.get<DragonInfoComponent>(e).color;
    registry.emplace<DragonHitComponent>(e, &GetAnimation(GetDragonAnimation(DragonAnimationType::HIT, color)));
}

void DragonBehaviorSystem::Init() {
}

void DragonBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, WalkingActorComponent, DragonComponent, DragonInfoComponent, RenderData>(entt::exclude<DragonHitComponent>);
    for (auto entity : view) {
        auto [pos, actor, dragon, dragonInfo, renderData] = view.get(entity);

#ifdef DEBUG_TOOLS
        if (Input::IsKeyPressed(KEY_U)) {
            dragon.invincibilityFramesLeft = 60 * 30;
        } else if (Input::IsKeyPressed(KEY_Y)) {
            dragon.invincibilityFramesLeft = 60 * 1000;
        }
#endif
        if (dragon.invincibilityFramesLeft > 0) {
            dragon.invincibilityFramesLeft--;

            if ((dragon.invincibilityFramesLeft / 2) % 2 == 0) {
                renderData.SetColor(COLOR_TRANSPARENT);
            } else {
                renderData.SetColor({255, 230, 200, 255});
            }
        }
        if (!dragon.isInvincible()) {
            renderData.SetColor(WHITE);
        }

        if (!dragon.isInvincible() && (collidesWithEnemy(registry, pos, Colliders::fullActorCollider) || collidesWithEnemyProjectile(registry, pos, Colliders::fullActorCollider) || collidesWithBoss(registry, pos, Colliders::fullActorCollider))) {
            Defer(entity, &makeDragonHit, 0);
            continue;
        }

        const int JUMP_FRAME_COUNT = 5 * (int)(UNITS_PER_BLOCK * 1.2f) / actor.jumpSpeed;

        int inputDir = Input::GetXAxis(dragonInfo.color);
        bool inputJump = Input::IsKeyDown(Key::Jump, dragonInfo.color);
        bool inputFire = Input::IsKeyDown(Key::Fire, dragonInfo.color);

        int moveSpeed = UNITS_PER_BLOCK / 16;
        if (GameModifierData::IsModifierSet(ModifierTypes::SPEED_UP)) {
            moveSpeed *= 2;

            pos.x = (pos.x / 2) * 2;
        }

        int velx = moveSpeed * inputDir;

        bool startedShootingAnimation = false;

        if (inputDir != 0) {
            pos.dir = inputDir;
        }

        // shooting
        if (dragon.bubbleShootDelay == 0 && inputFire) {
            EntityFactory::CreateBubbleCenteredAt(pos.toVector().Add(BP_SIZE(1, 0), BP_SIZE(1, 0)), pos.dir);
            dragon.bubbleShootDelay = dragon.MAX_BUBBLE_SHOOT_DELAY;
            if (GameModifierData::IsModifierSet(ModifierTypes::FIRERATE_UP)) {
                dragon.bubbleShootDelay /= 2;
            }

            startedShootingAnimation = true;

        } else if (dragon.bubbleShootDelay > 0) {
            dragon.bubbleShootDelay--;
        }

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, Colliders::walkingActorCollider);

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
        if (inputJump && !actor.isJumping()) {
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

                dragon.animator.SetNewAnimation(&GetAnimation(GetDragonAnimation(DragonAnimationType::SHOOTING, dragonInfo.color)));
                dragon.state = DragonComponent::SHOOTING;

            } else if (isGrounded) {

                if (inputDir == 0 && dragon.state != DragonComponent::IDLE) {
                    dragon.animator.SetNewAnimation(&GetAnimation(GetDragonAnimation(DragonAnimationType::IDLE, dragonInfo.color)));
                    dragon.state = DragonComponent::IDLE;

                } else if (inputDir != 0 && dragon.state != DragonComponent::WALKING) {
                    dragon.animator.SetNewAnimation(&GetAnimation(GetDragonAnimation(DragonAnimationType::WALKING, dragonInfo.color)));
                    dragon.state = DragonComponent::WALKING;
                }

            } else {

                if (actor.isJumping() && dragon.state != DragonComponent::JUMPING) {
                    dragon.animator.SetNewAnimation(&GetAnimation(GetDragonAnimation(DragonAnimationType::JUMPING, dragonInfo.color)));
                    dragon.state = DragonComponent::JUMPING;

                } else if (!actor.isJumping() && dragon.state != DragonComponent::FALLING) {
                    dragon.animator.SetNewAnimation(&GetAnimation(GetDragonAnimation(DragonAnimationType::FALLING, dragonInfo.color)));
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
