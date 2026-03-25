#include "GameplayWatcherSystem.h"
#include "entt/entity/fwd.hpp"

void GameplayWatcherSystem::Update() {
    auto view = registry.view<EnemyTag>();
    
    if (view->size() == 0) {
        eventSystem.Notify((entt::entity)0, ALL_ENEMIES_DEFEATED, 0);
    }
}
