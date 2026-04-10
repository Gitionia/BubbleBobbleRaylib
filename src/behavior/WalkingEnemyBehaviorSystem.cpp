#include "WalkingEnemyBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"
#include "entt/entity/fwd.hpp"

static void makeEnemyBubbled(entt::registry &registry, entt::entity e) {
    BubbleFloatComponent &c = registry.emplace<BubbleFloatComponent>(e);
    const EnemyInfoComponent &info = registry.get<EnemyInfoComponent>(e);
    c.animator.SetNewAnimation(&GetAnimation(GetEnemyAnimationName(info.type, EnemyAnimationType::BUBBLED)));
}

void WalkingEnemyBehaviorSystem::Init() {
}

void WalkingEnemyBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    bool foundDragon = false;
    Vector2Int dragonPos = {-1000, 0};
    auto dragonView = registry.view<Position, DragonTag>();
    for (entt::entity dragon : dragonView) {
        const Position &pos = registry.get<Position>(dragon);

        // If there are multiple dragon, select one by chance
        if (!foundDragon || Random::Get().Chance(0.5f)) {
            dragonPos = {pos.x, pos.y};
        }

        foundDragon = true;
        break;
    }

    if (!foundDragon) {
        PRINT_WARN("Walking enemy did not find dragon position");
    }

    auto view = registry.view<Position, WalkingActorComponent, WalkingEnemyComponent, EnemyInfoComponent, RenderData>(entt::exclude<BubbleFloatComponent, BubblePopComponent>);
    for (auto entity : view) {
        auto [pos, actor, enemy, info, renderData] = view.get(entity);

        std::optional<entt::entity> bubble = getCollidingShootingBubble(registry, pos, Colliders::fullActorCollider);
        if (bubble.has_value()) {
            Defer(entity, &makeEnemyBubbled, 0);
            Destroy(bubble.value());

            continue;
        }

        const bool isMushroom = info.type == EnemyType::MUSHROOM;
        const bool isSnowman = info.type == EnemyType::SNOWMAN;
        const bool canShoot = info.type == EnemyType::GHOST || info.type == EnemyType::POTATO || info.type == EnemyType::SNOWMAN || info.type == EnemyType::WITCH;

        const int FREEZE_FOR_JUMP_DURATION = 15;
        int FREEZE_FOR_SHOOT_DURATION = 30;
        int FREEZE_TIME_TO_SHOOT = 10;
        int SHOOTING_COOLDOWN = 80;

        if (info.type == EnemyType::GHOST) {
            FREEZE_FOR_SHOOT_DURATION = 30;
            FREEZE_TIME_TO_SHOOT = 10;
            SHOOTING_COOLDOWN = 90;

        } else if (info.type == EnemyType::POTATO) {
            FREEZE_FOR_SHOOT_DURATION = 10;
            FREEZE_TIME_TO_SHOOT = 0;
            SHOOTING_COOLDOWN = 30;

        } else if (info.type == EnemyType::WITCH) {
            FREEZE_FOR_SHOOT_DURATION = 100;
            FREEZE_TIME_TO_SHOOT = 40;
            SHOOTING_COOLDOWN = 240;

        } else if (info.type == EnemyType::SNOWMAN) {
            FREEZE_FOR_SHOOT_DURATION = 10;
            FREEZE_TIME_TO_SHOOT = 0;
            SHOOTING_COOLDOWN = 10;
        }

        int velx = 0;

        int moveSpeed = 2;
        if (info.type == EnemyType::SNOWMAN) {
            moveSpeed = BP_SIZE(0, 4);

            // snowman should always have pos.x be multiple of 4, so it can fit into 2 space gaps
            if (pos.x % 4 != 0) {
                pos.x = (pos.x / 4) * 4;
            }

        } else {
            moveSpeed = BP_SIZE(0, 2);
        }

        const int FALL_SPEED = 2 * UNITS_PER_BLOCK / 16;
        const int NORMAL_JUMP_SPEED = 3 * UNITS_PER_BLOCK / 16;
        const int GAP_JUMP_SPEED = 1 * UNITS_PER_BLOCK / 16;

        const int NORMAL_JUMP_FRAME_COUNT = BP_SIZE(5, 8) / NORMAL_JUMP_SPEED;
        const int GAP_JUMP_FRAME_COUNT = BP_SIZE(2, 8) / GAP_JUMP_SPEED;

        bool shouldGapJump = false;

        bool dragonIsAboveEnemy = dragonPos.Y < pos.y;
        bool dragonIsBelowEnemy = dragonPos.Y > pos.y;
        bool dragonAtSameYPos = dragonPos.Y == pos.y;
        int DRAGON_JUMP_TRIGGER_RADIUS = BP_SIZE(8, 0);
        bool dragonIsNear = pos.toVector().Dot(dragonPos) <= DRAGON_JUMP_TRIGGER_RADIUS * DRAGON_JUMP_TRIGGER_RADIUS;

        bool lookingAtDragon = sign(dragonPos.X - pos.x) == enemy.walkingDir;

        actor.fallSpeed = FALL_SPEED;

        if (enemy.freezeState == WalkingEnemyComponent::FREEZE_FOR_JUMP || (enemy.freezeState == WalkingEnemyComponent::FREEZE_FOR_SHOOT && enemy.freezeXPosDuration < FREEZE_TIME_TO_SHOOT)) {

            enemy.animator.GoToIndex(0);
        } else {
            enemy.animator.Update();
            if (enemy.animator.IsFinished()) {
                enemy.animator.Reset();
            }
        }

        renderData.spriteHandle = enemy.animator.GetSpriteHandle();

        // check if grounded
        bool isGrounded = false;
        if (!actor.isJumping()) {
            isGrounded = isWalkingActorGrounded(registry, pos, actor);

            // If we are grounded, but don't touch the floor, then round y-position
            if (isGrounded && pos.y % BP_SIZE(1, 0) != 0) {
                // We are at least one pixel above the ground
                pos.y = (pos.y / BP_SIZE(1, 0) + 1) * BP_SIZE(1, 0);
            }
        }

        if (!actor.isJumping() && enemy.isGapJumping) {
            // Transitioned from gap jumping to falling.
            // Then we should have no x-velocity
            enemy.isGapJumping = false;
            enemy.walkingDir = 0;
        }

        if (isGrounded) {

            enemy.isGapJumping = false;
            // just landed and didn't have a walking direction
            if (enemy.walkingDir == 0) {

                if (dragonPos.X == pos.x) {
                    enemy.walkingDir = Random::Get().GetDirection();

                } else {
                    // Most of the time choose direction to player
                    enemy.walkingDir = sign(dragonPos.X - pos.x);
                    if (Random::Get().Chance(0.33f)) {
                        enemy.walkingDir *= -1;
                    }
                }
            }

            pos.dir = enemy.walkingDir;

        } else if (!enemy.isGapJumping) {
            enemy.walkingDir = 0;
        }

        // Handle freezing
        if (enemy.shootCooldown > 0) {
            enemy.shootCooldown--;
        }
        if (enemy.freezeXPosDuration > 0) {
            enemy.freezeXPosDuration--;

            if (enemy.freezeXPosDuration == FREEZE_TIME_TO_SHOOT && enemy.freezeState == WalkingEnemyComponent::FREEZE_FOR_SHOOT) {
                EntityFactory::CreateEnemyProjectile(pos.x, pos.y, pos.dir, info.type);
                enemy.shootCooldown = SHOOTING_COOLDOWN;

            } else if (enemy.freezeXPosDuration == 0 && enemy.freezeState == WalkingEnemyComponent::FREEZE_FOR_SHOOT) {
                enemy.animator.SetNewAnimation(&GetAnimation(GetEnemyAnimationName(info.type, EnemyAnimationType::NORMAL)));

                enemy.freezeState = WalkingEnemyComponent::NOT_FREEZING;

            } else if (enemy.freezeXPosDuration == 0 && enemy.freezeState == WalkingEnemyComponent::FREEZE_FOR_JUMP) {
                // Turn around and check if this was the last turn around before the jump.
                // If so then jump, otherwise freeze again
                enemy.walkingDir *= -1;
                enemy.jumpTurnAroundsCount--;

                if (enemy.jumpTurnAroundsCount == 0) {
                    actor.jumpSpeed = NORMAL_JUMP_SPEED;
                    actor.jumpFrameCount = NORMAL_JUMP_FRAME_COUNT;

                    enemy.freezeState = WalkingEnemyComponent::NOT_FREEZING;

                } else {
                    enemy.setFreezing(FREEZE_FOR_JUMP_DURATION, WalkingEnemyComponent::FREEZE_FOR_JUMP);
                }
            }

            // else (not frozen): check if should start shooting
        } else if (isSnowman) {
            if (enemy.freezeXPosDuration == 0 && enemy.shootCooldown == 0) {
                // snowman doesn't freeze while shooting (obviously!)
                if (Random::Get().Chance(0.01f)) {
                    enemy.shootCooldown = SHOOTING_COOLDOWN;
                    EntityFactory::CreateEnemyProjectile(pos.x, pos.y, pos.dir, info.type);
                }
            }
        } else {
            if (canShoot && isGrounded && dragonAtSameYPos && lookingAtDragon && enemy.freezeXPosDuration == 0 && enemy.shootCooldown == 0) {
                if (Random::Get().Chance(0.05f)) {
                    enemy.setFreezing(FREEZE_FOR_SHOOT_DURATION, WalkingEnemyComponent::FREEZE_FOR_SHOOT);
                    enemy.animator.SetNewAnimation(&GetAnimation(GetEnemyAnimationName(info.type, EnemyAnimationType::SHOOTING)));
                }
            }
        }

        // check if should gap jump (mushrooms should always gap jump)
        if (isGrounded && !enemy.isFreezing()) {
            if (isSnowman) {
                shouldGapJump = false;
            } else if (isMushroom) {
                shouldGapJump = true;
            } else {
                shouldGapJump = shouldWalkingEnemyGapJump(pos, enemy.walkingDir);
            }
        }

        if (enemy.isFreezing()) {
            velx = 0;

        } else {
            velx = moveSpeed * enemy.walkingDir;
        }

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, Colliders::walkingActorCollider);

        // start jump (snowmen can't jump)
        if (isGrounded && !actor.isJumping() && !enemy.isFreezing() && !isSnowman) {

            if (isMushroom) {
                if (dragonIsAboveEnemy && Random::Get().Chance(0.84f) || dragonAtSameYPos && Random::Get().Chance(0.92f) || dragonIsBelowEnemy && Random::Get().Chance(0.98f)) {
                    enemy.animator.Reset();
                    enemy.isGapJumping = true;
                    actor.jumpSpeed = GAP_JUMP_SPEED;
                    actor.jumpFrameCount = GAP_JUMP_FRAME_COUNT;
                } else {
                    enemy.isGapJumping = false;
                    enemy.jumpTurnAroundsCount = 3;
                    enemy.setFreezing(FREEZE_FOR_JUMP_DURATION, WalkingEnemyComponent::FREEZE_FOR_JUMP);
                }

            } else {
                if (shouldGapJump && (dragonIsAboveEnemy && Random::Get().Chance(0.05f) || dragonAtSameYPos && (Random::Get().Chance(0.08f)) || dragonIsBelowEnemy && (Random::Get().Chance(0.015f)))) {
                    enemy.isGapJumping = true;
                    actor.jumpSpeed = GAP_JUMP_SPEED;
                    actor.jumpFrameCount = GAP_JUMP_FRAME_COUNT;

                } else if (dragonIsAboveEnemy && Random::Get().Chance(0.011f) || dragonAtSameYPos && (Random::Get().Chance(0.005f)) || dragonIsBelowEnemy && (Random::Get().Chance(0.005f))) {
                    enemy.isGapJumping = false;
                    enemy.jumpTurnAroundsCount = 3;
                    enemy.setFreezing(FREEZE_FOR_JUMP_DURATION, WalkingEnemyComponent::FREEZE_FOR_JUMP);
                }
            }
        }

        if (enemy.isGapJumping && actor.isJumping() && actor.jumpFrameCount <= GAP_JUMP_FRAME_COUNT / 2) {
            actor.jumpSpeed = 0;
        }

        // handle x movement
        if (!enemy.isFreezing()) {
            pos.x += velx;
            if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
                // Round pos.x to full block position
                pos.x = (pos.x / BP_SIZE(1, 0)) * BP_SIZE(1, 0);
                if (enemy.walkingDir == -1) {
                    pos.x += BP_SIZE(1, 0);
                }

                // gap jumping enemies should fall down at the wall and not bounce off
                if (enemy.isGapJumping) {
                    if (isMushroom) {
                        enemy.walkingDir *= -1;
                    } else {
                        enemy.walkingDir = 0;
                    }

                } else {
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
    }
}
