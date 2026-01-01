#include "Systems.h"

#include "../behavior/TrashCanBehaviorSystem.h"
#include "Components.h"

#include "../behavior/BubbleBehaviorSystem.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../graphics/RendererSystem.h"
#include "../behavior/BubbleBehaviorSystem.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../graphics/RendererSystem.h"
#include "../behavior/WalkingActorBehaviorSystem.h"


SystemRunner::SystemRunner(entt::registry &registry)
    : registry(registry) {
        
    registerSystem<TrashCanBehaviorSystem>(registry);
    registerSystem<DragonBehaviorSystem>(registry);
    registerSystem<WalkingActorBehaviorSystem>(registry);
    registerSystem<BubbleBehaviorSystem>(registry);
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
        system->Update();
    }

    auto view = registry.view<DestroyEntity>();
    registry.destroy(view.begin(), view.end());
}

template <typename T>
void SystemRunner::registerSystem(entt::registry &registry) {
    T *system = new T(registry);
    systems.push_back(system);
}
