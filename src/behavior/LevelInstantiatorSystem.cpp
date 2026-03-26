#include "LevelInstantiatorSystem.h"
#include "../level/Physics.h"
#include "entt/entity/fwd.hpp"

void LevelInstantiatorSystem::Update() {
    const auto &events = eventSystem.ReadEvent(INSTANTIATE_LEVEL);
    DBG_CHECK(events.size() == 1, 
        std::format("Expected event count = 1, but was {} for instantiating new levels. Probably a bug?", events.size()));

    if (!events.empty()) {
        int newLevel = events.at(0).data;

        clearExistingLevel();
        loadNewLevel(newLevel);

    }
}

void LevelInstantiatorSystem::clearExistingLevel() {
    auto view = registry.view<GameplayEntityTag>();
    registry.destroy(view->begin(), view->end());
}

void LevelInstantiatorSystem::loadNewLevel(int levelNumber) {
    level = LevelLayout::LoadLevel(std::format("res/levels/Level{}.json", levelNumber));
    EntityFactory::CreateLevel(level);
    setPhysicsColliderData(level);
    auto dragon = EntityFactory::CreateDragon();
}
