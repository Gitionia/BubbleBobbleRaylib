#include "Systems.h"

#include "Components.h"

#include "../behavior/TrashCanBehaviorSystem.h"
#include "../behavior/EnemyTumbleBehaviorSystem.h"
#include "../behavior/PopEnemyBubbleSystem.h"
#include "../behavior/BubbleFloatBehaviorSystem.h"
#include "../behavior/BubblePopBehaviorSystem.h"
#include "../behavior/BubbleShootBehaviorSystem.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../behavior/DragonHitBehaviorSystem.h"
#include "../behavior/WalkingActorBehaviorSystem.h"
#include "../graphics/RendererSystem.h"

SystemRunner::SystemRunner(entt::registry &registry)
    : registry(registry) {

    registerSystem<TrashCanBehaviorSystem>(registry);
    registerSystem<PopEnemyBubbleSystem>(registry);
    registerSystem<EnemyTumbleBehaviorSystem>(registry);
    registerSystem<DragonBehaviorSystem>(registry);
    registerSystem<DragonHitBehaviorSystem>(registry);
    registerSystem<WalkingActorBehaviorSystem>(registry);
    registerSystem<BubbleShootBehaviorSystem>(registry);
    registerSystem<BubbleFloatBehaviorSystem>(registry);
    registerSystem<BubblePopBehaviorSystem>(registry);
    registerSystem<RendererSystem>(registry);
}

SystemRunner::~SystemRunner() {
    for (auto system : systems) {
        delete system;
    }
}

void SystemRunner::Init() {
    for (auto system : systems) {
        system->Init();
    }
}

void SystemRunner::UpdateSystems() const {
    for (auto system : systems) {
        system->BaseUpdate();
    }
}

template <typename T>
void SystemRunner::registerSystem(entt::registry &registry) {
    T *system = new T(registry);
    systems.push_back(system);
}
