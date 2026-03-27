#include "FlyingEnemyBehaviorSystem.h"
#include "TrashCanBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"

static void makeEnemyBubbled(entt::registry &registry, entt::entity e) {
    BubbleFloatComponent &c = registry.emplace<BubbleFloatComponent>(e);
    const EnemyInfoComponent& info = registry.get<EnemyInfoComponent>(e);
    c.animator.SetNewAnimation(&GetAnimation(GetEnemyAnimationName(info.type, EnemyAnimationType::BUBBLED)));
}

void FlyingEnemyBehaviorSystem::Init() {
}

void FlyingEnemyBehaviorSystem::Update() {

    const Collider &collider = Colliders::fullActorCollider;

    static Animator animator(&GetAnimation("Purple-Fly"));

    animator.Update();
    if (animator.IsFinished()) {
        animator.Reset();
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

        renderData.spriteHandle = animator.GetSpriteHandle();

        int moveSpeed = UNITS_PER_BLOCK / 16;

        int xVel = moveSpeed * enemy.getXDir();
        int yVel = moveSpeed * enemy.getYDir();

        pos.x += xVel;
        if (collidesWithWall(registry, pos, collider)) {
            pos.x -= xVel;
            enemy.flipX();
        }
        pos.y += yVel;
        if (collidesWithWall(registry, pos, collider)) {
            pos.y -= yVel;
            enemy.flipY();
        }

        pos.dir = enemy.getXDir();
    }
}
