#include "TrashCanBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"

static void makeEnemyBubbled(entt::registry &registry, entt::entity e) {
    BubbleFloatComponent &c = registry.emplace<BubbleFloatComponent>(e);
    const EnemyInfoComponent &info = registry.get<EnemyInfoComponent>(e);
    c.animator.SetNewAnimation(&GetAnimation(GetEnemyAnimationName(info.type, EnemyAnimationType::BUBBLED)));
}

void TrashCanBehaviorSystem::Init() {
}

void TrashCanBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    static Animator animator(&GetAnimation("Can-Walk"));

    animator.Update();
    if (animator.IsFinished()) {
        animator.Reset();
    }

    auto view = registry.view<Position, WalkingActorComponent, WalkingEnemyComponent, RenderData>(entt::exclude<BubbleFloatComponent, BubblePopComponent>);
    for (auto entity : view) {
        auto [pos, actor, enemy, renderData] = view.get(entity);

        std::optional<entt::entity> bubble = getCollidingShootingBubble(registry, pos, Colliders::fullActorCollider);
        if (bubble.has_value()) {
            Defer(entity, &makeEnemyBubbled, 0);
            Destroy(bubble.value());

            continue;
        }

        renderData.spriteHandle = animator.GetSpriteHandle();

        int velx = 0;
        int moveSpeed = 2 * UNITS_PER_BLOCK / 16;

        const int FALL_SPEED = UNITS_PER_BLOCK / 16;
        const int NORMAL_JUMP_SPEED = 3 * UNITS_PER_BLOCK / 16;
        const int GAP_JUMP_SPEED = 2 * UNITS_PER_BLOCK / 16;

        const int NORMAL_JUMP_FRAME_COUNT = BP_SIZE(5, 8) / NORMAL_JUMP_SPEED;
        const int GAP_JUMP_FRAME_COUNT = BP_SIZE(2, 8) / GAP_JUMP_SPEED;

        bool shouldGapJump = false;

        // check if grounded
        bool isGrounded = false;
        if (!actor.isJumping()) {
            isGrounded = isWalkingActorGrounded(registry, pos, actor);
        }

        if (isGrounded) {

            enemy.isGapJumping = false;
            // just landed and didn't have a walking direction
            if (enemy.walkingDir == 0) {
                enemy.walkingDir = Random::Get().GetDirection();
            }

            pos.dir = enemy.walkingDir;

        } else if (!enemy.isGapJumping) {
            enemy.walkingDir = 0;
        }

        if (isGrounded) {
            shouldGapJump = shouldWalkingEnemyGapJump(pos, enemy.walkingDir);
        }

        velx = moveSpeed * enemy.walkingDir;

        actor.ignoreCollisions = shouldWalkingActorIgnoreCollisions(registry, pos, Colliders::walkingActorCollider);

        // TODO: add check for if player is above or below
        // start jump
        if (isGrounded) {

            if (shouldGapJump && Random::Get().Chance(0.15f)) {
                enemy.isGapJumping = true;
                actor.jumpSpeed = GAP_JUMP_SPEED;
                actor.jumpFrameCount = GAP_JUMP_FRAME_COUNT;

            } else if (Random::Get().Chance(0.001f)){
                enemy.isGapJumping = false;
                actor.jumpSpeed = NORMAL_JUMP_SPEED;
                actor.jumpFrameCount = NORMAL_JUMP_FRAME_COUNT;
            }
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
                enemy.walkingDir = 0;

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
