#include "PopEnemyBubbleSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"

enum DeferValues {
    RevertEnemyToNormal = 0,
    MakeEnemyTumble = 1
};

void makeEnemyNormal(entt::registry& registry, entt::entity entity) {
    // Note: We remove Enemy tag from original entity and spawn a new enemy
    // There could be potential issues if the original entity has components,
    // that are picked up by systems that we don't want them to

    registry.emplace<EnemyBubblePoppedTag>(entity);

    Position pos = registry.get<Position>(entity);
    pos.x -= BP_SIZE(0, 2);
    EnemyInfoComponent info = registry.get<EnemyInfoComponent>(entity);
    EntityFactory::CreateEnemy(pos.x, pos.y, info.type, GetDirectionFromInt(Random::Get().GetDirection()));
}

void makeEnemyTumble(entt::registry& registry, entt::entity entity) {
    registry.emplace<EnemyBubblePoppedTag>(entity);

    Position pos = registry.get<Position>(entity);
    pos.x -= BP_SIZE(0, 2);

    BubblePopComponent& popComp = registry.get<BubblePopComponent>(entity);

    const EnemyInfoComponent& info = registry.get<EnemyInfoComponent>(entity);
    EntityFactory::CreateTumblingEnemy(pos.x, pos.y, Random::Get().GetDirection(), info.type, GetItemOfLevel(popComp.itemLevel));
}

void PopEnemyBubbleSystem::Update() {

    auto view = registry.view<BubblePopComponent, EnemyTag>(entt::exclude<EnemyBubblePoppedTag>);
    for (auto entity : view) {
        auto [pop] = view.get(entity);
    
        if (pop.poppedFromLifeTime) {
            Defer(entity, &makeEnemyNormal, RevertEnemyToNormal);

        } else {
            Defer(entity, &makeEnemyTumble, MakeEnemyTumble);
        }
    }
}
