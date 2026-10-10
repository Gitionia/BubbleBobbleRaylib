#include "Systems.h"

#include "../behavior/BossBehaviorSystem.h"
#include "../behavior/BubbleFloatBehaviorSystem.h"
#include "../behavior/BubblePopBehaviorSystem.h"
#include "../behavior/BubbleShootBehaviorSystem.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../behavior/DragonHitBehaviorSystem.h"
#include "../behavior/EnemyAppearanceBehaviorSystem.h"
#include "../behavior/EnemyProjectileBehaviorSystem.h"
#include "../behavior/EnemyTumbleBehaviorSystem.h"
#include "../behavior/FlyingEnemyBehaviorSystem.h"
#include "../behavior/GameplayUISystem.h"
#include "../behavior/GameplayWatcherSystem.h"
#include "../behavior/IntroSceneBehaviorSystem.h"
#include "../behavior/VictorySceneBehaviorSystem.h"
#include "../behavior/ItemPickupBehaviorSystem.h"
#include "../behavior/LevelInstantiatorSystem.h"
#include "../behavior/PopEnemyBubbleSystem.h"
#include "../behavior/PositionAnimationBehaviorSystem.h"
#include "../behavior/TitleScreenSystem.h"
#include "../behavior/WalkingActorBehaviorSystem.h"
#include "../behavior/WalkingEnemyBehaviorSystem.h"
#include "../graphics/RendererSystem.h"
#include "../level/GameModifiers.h"
#include "SystemBase.h"

SystemRunner::SystemRunner(entt::registry &registry, EventSystem &eventSystem)
    : registry(registry), eventSystem(eventSystem) {

    registerSystem<EnemyAppearanceBehaviorSystem>();
    registerSystem<WalkingEnemyBehaviorSystem>();
    registerSystem<BossBehaviorSystem>();
    registerSystem<EnemyProjectileBehaviorSystem>();
    registerSystem<FlyingEnemyBehaviorSystem>();
    registerSystem<PopEnemyBubbleSystem>();
    registerSystem<EnemyTumbleBehaviorSystem>();
    registerSystem<DragonBehaviorSystem>();
    registerSystem<DragonHitBehaviorSystem>();
    registerSystem<WalkingActorBehaviorSystem>();
    registerSystem<BubbleShootBehaviorSystem>();
    registerSystem<BubbleFloatBehaviorSystem>();
    registerSystem<BubblePopBehaviorSystem>();
    registerSystem<ItemPickupBehaviorSystem>();
    registerSystem<PositionAnimationBehaviorSystem>();
    registerSystem<GameplayUISystem>();
    registerSystem<GameplayWatcherSystem>();
    registerSystem<LevelInstantiatorSystem>();
    registerSystem<TitleScreenSystem>();
    registerSystem<IntroSceneBehaviorSystem>();
    registerSystem<VictorySceneBehaviorSystem>();
    registerSystem<GameModifierData>();
    registerSystem<RendererSystem>();
}

void SystemRunner::Init() {
    for (auto system : systems) {
        system->Init();
    }
}

void SystemRunner::SetupAllSystems() {
    for (auto system : systems) {
        system->Setup();
    }
}

void SystemRunner::SetupOnlyEnabledSystems() {
    for (auto system : systems) {
        if (system->IsEnabled()) {
            system->Setup();
        }
    }
}

void SystemRunner::UpdateSystems() const {
    for (auto system : systems) {
        system->BaseUpdate();
    }
}

void SystemRunner::OnlyHaveSystemsEnabledThatMatchAnyFlag(int flags) {
    for (auto system : systems) {
        system->SetEnabledIfMatchesAnyFlag(flags);
    }
}

template <typename T>
std::shared_ptr<SystemBase> SystemRunner::CreateSystem() {
    std::shared_ptr<T> system = std::make_shared<T>(registry, eventSystem);
    return system;
}

template <typename T>
void SystemRunner::registerSystem() {
    systems.push_back(CreateSystem<T>());
}
