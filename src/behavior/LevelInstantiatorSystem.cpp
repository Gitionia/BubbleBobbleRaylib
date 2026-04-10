#include "LevelInstantiatorSystem.h"
#include "../level/Physics.h"
#include "entt/entity/fwd.hpp"
#include <string>

void LevelInstantiatorSystem::Update() {
    if (eventSystem.ReadEvent(DELETE_GAMEPLAY).size()) {
        clearExistingLevel();

    } else {
        const auto &events = eventSystem.ReadEvent(INSTANTIATE_LEVEL);
        bool didSomething = false;
        DBG_CHECK(events.size() <= 1, std::format("Expected INSTANTIATE_LEVEL event count <= 1, but was {} for instantiating new levels. Probably a bug?", events.size()));

        if (!events.empty()) {
            int newLevel = events.at(0).data;

            bool addSecondPlayer = eventSystem.ReadEvent(INSTANTIATE_ADDITIONAL_PLAYER).size();

            clearExistingLevel();
            loadNewLevel(newLevel, addSecondPlayer);

            didSomething = true;
        }

        if (eventSystem.ReadEvent(INSTANTIATE_ADDITIONAL_PLAYER).size()) {
            if (registry.view<DragonTag>()->size() < 2) {
                auto dragon = EntityFactory::CreateDragon(DRAGON_BLUE);
            }

            didSomething = true;
        }
        if (eventSystem.ReadEvent(DESTROY_ADDITIONAL_PLAYER).size()) {
            auto view = registry.view<DragonInfoComponent>();
            if (view->size() >= 2) {
                for (entt::entity entity : view) {
                    DragonInfoComponent dragonInfo = registry.get<DragonInfoComponent>(entity);
                    if (dragonInfo.color == DRAGON_BLUE) {
                        Destroy(entity);
                    }
                }
            }

            didSomething = true;
        }

        DBG_CHECK(didSomething, "Used LevelInstantiatorSystem, but it didn't have to do anything. Probably a bug?");
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

void LevelInstantiatorSystem::loadNewLevel(int levelNumber, bool addSecondPlayer) {

    level = LevelLayout::LoadLevel(std::format("res/levels/Level{}.json", convertLevelNumber(levelNumber)));
    EntityFactory::CreateLevel(level, levelNumber);

    if (level.ContainsBoss()) {
        EntityFactory::CreateEnemy(BP_SIZE(10, 0), BP_SIZE(12, 0), EnemyType::BOSS, Direction::Left, false);
    }

    setPhysicsColliderData(level);
    auto dragon = EntityFactory::CreateDragon(DRAGON_GREEN);
    if (addSecondPlayer) {
        auto dragon = EntityFactory::CreateDragon(DRAGON_BLUE);
    }
}
