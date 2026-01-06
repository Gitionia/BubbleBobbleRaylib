#include "PopEnemyBubbleSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "Physics.h"
#include "WalkingActorUtils.h"

enum DeferValues {
    RevertEnemyToNormal = 0,
    MakeEnemyItemDice = 1
};

void makeEnemyNormal(entt::registry& registry, entt::entity entity) {
    // Note: We remove Enemy tag from original entity and spawn a new enemy
    // There could be potential issues if the original entity has components,
    // that are picked up by systems that we don't want them to

    registry.emplace<EnemyBubblePoppedTag>(entity);

    Position pos = registry.get<Position>(entity);
    pos.x -= BP_SIZE(0, 2);
    EntityFactory::CreateEnemy(pos.x, pos.y);
}

void makeEnemyItemDice(entt::registry& registry, entt::entity entity) {
    registry.emplace<EnemyBubblePoppedTag>(entity);

    Position pos = registry.get<Position>(entity);
    pos.x -= BP_SIZE(0, 2);
    EntityFactory::CreateEnemyDice(pos.x, pos.y, pos.dir);
}

void PopEnemyBubbleSystem::Update() {

    auto view = registry.view<BubblePopComponent, EnemyTag>(entt::exclude<EnemyBubblePoppedTag>);
    for (auto entity : view) {
        auto [pop] = view.get(entity);
    
        if (pop.poppedFromLifeTime) {
            Defer(entity, &makeEnemyNormal, RevertEnemyToNormal);

        } else {
            Defer(entity, &makeEnemyItemDice, MakeEnemyItemDice);
        }
    }
}
