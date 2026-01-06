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

    auto view = registry.view<Position, WalkingActorComponent, EnemyComponent, RenderData>(entt::exclude<BubbleFloatComponent, BubblePopComponent>);
    for (auto entity : view) {
        auto [pos, actor, enemy, renderData] = view.get(entity);

        
    }
}
