#include "LevelInstantiatorSystem.h"
#include "../level/Physics.h"
#include "entt/entity/fwd.hpp"
#include <string>

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

static std::string convertLevelNumber(int levelNumber) {
    if (levelNumber >= 100) {
        return std::to_string(levelNumber);
    } else if (levelNumber >= 10) {
        return "0" + std::to_string(levelNumber);
    } else {
        return "00" + std::to_string(levelNumber);
    }
}

void LevelInstantiatorSystem::loadNewLevel(int levelNumber) {

    level = LevelLayout::LoadLevel(std::format("res/levels/Level{}.json", convertLevelNumber(levelNumber)));
    EntityFactory::CreateLevel(level, levelNumber);
    
    if (level.ContainsBoss()) {
        EntityFactory::CreateEnemy(BP_SIZE(10, 0),BP_SIZE(12, 0), EnemyType::BOSS, Direction::Left);
    }

    setPhysicsColliderData(level);
    auto dragon = EntityFactory::CreateDragon();
}
