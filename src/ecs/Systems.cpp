#include "Systems.h"


#include "../behavior/TrashCanBehaviorSystem.h"
#include "../behavior/EnemyTumbleBehaviorSystem.h"
#include "../behavior/PopEnemyBubbleSystem.h"
#include "../behavior/BubbleFloatBehaviorSystem.h"
#include "../behavior/BubblePopBehaviorSystem.h"
#include "../behavior/BubbleShootBehaviorSystem.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../behavior/DragonHitBehaviorSystem.h"
#include "../behavior/WalkingActorBehaviorSystem.h"
#include "../behavior/ItemPickupBehaviorSystem.h"
#include "../behavior/PositionAnimationBehaviorSystem.h"
#include "../behavior/GameplayUISystem.h"
#include "../graphics/RendererSystem.h"

SystemRunner::SystemRunner(entt::registry &registry, EventSystem& eventSystem)
    : registry(registry) {

    registerSystem<TrashCanBehaviorSystem>(registry, eventSystem);
    registerSystem<PopEnemyBubbleSystem>(registry, eventSystem);
    registerSystem<EnemyTumbleBehaviorSystem>(registry, eventSystem);
    registerSystem<DragonBehaviorSystem>(registry, eventSystem);
    registerSystem<DragonHitBehaviorSystem>(registry, eventSystem);
    registerSystem<WalkingActorBehaviorSystem>(registry, eventSystem);
    registerSystem<BubbleShootBehaviorSystem>(registry, eventSystem);
    registerSystem<BubbleFloatBehaviorSystem>(registry, eventSystem);
    registerSystem<BubblePopBehaviorSystem>(registry, eventSystem);
    registerSystem<ItemPickupBehaviorSystem>(registry, eventSystem);
    registerSystem<PositionAnimationBehaviorSystem>(registry, eventSystem);
    registerSystem<GameplayUISystem>(registry, eventSystem);
    registerSystem<RendererSystem>(registry, eventSystem);
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
void SystemRunner::registerSystem(entt::registry &registry, EventSystem& eventSystem) {
    T *system = new T(registry, eventSystem);
    systems.push_back(system);
}
