#include "EnemyTumbleBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "Physics.h"
#include "WalkingActorUtils.h"

void EnemyTumbleBehaviorSystem::Init() {
}

void EnemyTumbleBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, EnemyTumbleComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, enemy, renderData] = view.get(entity);

        pos.x += enemy.xVel.get() * pos.dir;
        pos.y += -enemy.yVel.get();

        enemy.xVel.tick();
        enemy.yVel.tick();

        enemy.animator.Update();
        if (enemy.animator.IsFinished()) {
            enemy.animator.Reset();
        }

        renderData.spriteHandle = enemy.animator.GetSpriteHandle();
    }
}
