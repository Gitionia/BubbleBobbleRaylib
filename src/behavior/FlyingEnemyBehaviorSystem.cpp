#include "FlyingEnemyBehaviorSystem.h"

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

void FlyingEnemyBehaviorSystem::Init() {
}

void FlyingEnemyBehaviorSystem::Update() {

    const Collider &collider = Colliders::fullActorCollider;

    static Animator purpleAnimator(&GetAnimation(GetEnemyAnimationName(EnemyType::PURPLE_GHOST, EnemyAnimationType::NORMAL)));
    static Animator pigAnimator(&GetAnimation(GetEnemyAnimationName(EnemyType::PIG, EnemyAnimationType::NORMAL)));

    purpleAnimator.Update();
    if (purpleAnimator.IsFinished()) {
        purpleAnimator.Reset();
    }
    pigAnimator.Update();
    if (pigAnimator.IsFinished()) {
        pigAnimator.Reset();
    }

    auto view = registry.view<Position, FlyingEnemyComponent, EnemyInfoComponent, RenderData>(entt::exclude<BubbleFloatComponent, BubblePopComponent>);
    for (auto entity : view) {
        auto [pos, enemy, info, renderData] = view.get(entity);

        std::optional<entt::entity> bubble = getCollidingShootingBubble(registry, pos, Colliders::fullActorCollider);
        if (bubble.has_value()) {
            Defer(entity, &makeEnemyBubbled, 0);
            Destroy(bubble.value());

            continue;
        }

        renderData.spriteHandle = (info.type == EnemyType::PURPLE_GHOST ? purpleAnimator : pigAnimator).GetSpriteHandle();

        int speedX = BP_SIZE(0, 2);
        int speedY = info.type == EnemyType::PURPLE_GHOST ? BP_SIZE(0, 2) : BP_SIZE(0, 1);

        int xVel = speedX * enemy.getXDir();
        int yVel = speedY * enemy.getYDir();

        pos.x += xVel;
        if (collidesWithWall(registry, pos, enemy.getVerticalCollider()) || collidesWithWall(registry, pos, enemy.getHorizontalCollider())) {
            pos.x -= xVel;
            enemy.flipX();
        }
        pos.y += yVel;
        if (collidesWithWall(registry, pos, enemy.getVerticalCollider()) || collidesWithWall(registry, pos, enemy.getHorizontalCollider())) {
            pos.y -= yVel;
            enemy.flipY();
        }

        // apply warping
        if (pos.y >= BP_SIZE(LevelTilemap::HEIGHT + 1, 0)) {
            pos.y = BP_SIZE(-2, 0);
        } else if (pos.y < BP_SIZE(-3, 0)) {
            pos.y = BP_SIZE(LevelTilemap::HEIGHT, 0);
        }

        Debug::DrawCollider(pos.x, pos.y, enemy.getHorizontalCollider());
        Debug::DrawCollider(pos.x, pos.y, enemy.getVerticalCollider());

        pos.dir = enemy.getXDir();
    }
}
