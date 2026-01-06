#include "EnemyItemDiceBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "Physics.h"
#include "WalkingActorUtils.h"

void EnemyItemDiceBehaviorSystem::Init() {
}

void EnemyItemDiceBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, EnemyDiceComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, enemy, renderData] = view.get(entity);

        enemy.animator.Update();
        if (enemy.animator.IsFinished()) {
            enemy.animator.Reset();
        }

        renderData.spriteHandle = enemy.animator.GetSpriteHandle();
    }
}
