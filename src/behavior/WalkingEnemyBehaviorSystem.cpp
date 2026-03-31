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

    static Animator canAnimator(&GetAnimation(GetEnemyAnimationName(EnemyType::CAN, EnemyAnimationType::NORMAL)));
    static Animator ghostAnimator(&GetAnimation(GetEnemyAnimationName(EnemyType::GHOST, EnemyAnimationType::NORMAL)));

    canAnimator.Update();
    if (canAnimator.IsFinished()) {
        canAnimator.Reset();
    }

    ghostAnimator.Update();
    if (ghostAnimator.IsFinished()) {
        ghostAnimator.Reset();
    }

    bool foundDragon = false;
    Vector2Int dragonPos = {-1000, 0};
    auto dragonView = registry.view<Position, DragonTag>();
    for (entt::entity dragon : dragonView) {
        const Position &pos = registry.get<Position>(dragon);
        dragonPos = {pos.x, pos.y};

        foundDragon = true;
        // If there are multiple dragons, maybe take the highest position?
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

        switch (info.type) {

        case EnemyType::CAN:
            renderData.spriteHandle = canAnimator.GetSpriteHandle();
            break;
        case EnemyType::GHOST:
            renderData.spriteHandle = ghostAnimator.GetSpriteHandle();
            break;
        case EnemyType::MUSHROOM:
            // Create flapping animation by repeating the last to frames
            if (!enemy.animator.IsFinished()) {
                enemy.animator.Update();
            } else {
                enemy.animator.GoToIndex(enemy.animator.GetAnimationSpriteCount() - 2);
            }
            renderData.spriteHandle = enemy.animator.GetSpriteHandle();
            break;
        case EnemyType::SNOWMAN:
        case EnemyType::POTATO:
        case EnemyType::WITCH:
            PRINT_WARN("Animator for walking enemy {} is not implemented yet!", (int)info.type);
            break;
        default:
            PRINT_WARN("Walking enemy has enemy type {}, which is not a walking enemy type!", (int)info.type);
            break;
        }

        const bool isMushroom = info.type == EnemyType::MUSHROOM;

        int velx = 0;
        int moveSpeed = 2 * UNITS_PER_BLOCK / 16;

        const int FALL_SPEED = 2 * UNITS_PER_BLOCK / 16;
        const int NORMAL_JUMP_SPEED = 3 * UNITS_PER_BLOCK / 16;
        const int GAP_JUMP_SPEED = 1 * UNITS_PER_BLOCK / 16;

        const int NORMAL_JUMP_FRAME_COUNT = BP_SIZE(5, 8) / NORMAL_JUMP_SPEED;
        const int GAP_JUMP_FRAME_COUNT = BP_SIZE(2, 8) / GAP_JUMP_SPEED;

        bool shouldGapJump = false;

        bool dragonIsAboveEnemy = dragonPos.Y < pos.y;
        int DRAGON_JUMP_TRIGGER_RADIUS = BP_SIZE(8, 0);
        bool dragonIsNear = pos.toVector().Dot(dragonPos) <= DRAGON_JUMP_TRIGGER_RADIUS * DRAGON_JUMP_TRIGGER_RADIUS;

        actor.fallSpeed = FALL_SPEED;

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
                    if (Random::Get().Chance(0.25f)) {
                        enemy.walkingDir *= -1;
                    }
                }
            }

            pos.dir = enemy.walkingDir;

        } else if (!enemy.isGapJumping) {
            enemy.walkingDir = 0;
        }

        // check if should gap jump (mushrooms don't need to check that since they always can)
        if (isGrounded && !isMushroom) {
            shouldGapJump = shouldWalkingEnemyGapJump(pos, enemy.walkingDir);
        }

        velx = moveSpeed * enemy.walkingDir;

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, Colliders::walkingActorCollider);

        // start jump
        if (isGrounded) {

            int chanceMultiplier = dragonIsAboveEnemy ? dragonIsNear ? 10 : 5 : 1;
            if (isMushroom && Random::Get().Chance(chanceMultiplier * 0.3f)) {
                enemy.animator.Reset();
                enemy.isGapJumping = true;
                actor.jumpSpeed = GAP_JUMP_SPEED;
                actor.jumpFrameCount = GAP_JUMP_FRAME_COUNT;

            } else if (shouldGapJump && Random::Get().Chance(chanceMultiplier * 0.01f)) {
                enemy.isGapJumping = true;
                actor.jumpSpeed = GAP_JUMP_SPEED;
                actor.jumpFrameCount = GAP_JUMP_FRAME_COUNT;

            } else if (Random::Get().Chance(chanceMultiplier * 0.004f)) {
                enemy.isGapJumping = false;
                actor.jumpSpeed = NORMAL_JUMP_SPEED;
                actor.jumpFrameCount = NORMAL_JUMP_FRAME_COUNT;
            }
        }

        if (enemy.isGapJumping && actor.isJumping() && actor.jumpFrameCount <= GAP_JUMP_FRAME_COUNT / 2) {
            actor.jumpSpeed = 0;
        }

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
