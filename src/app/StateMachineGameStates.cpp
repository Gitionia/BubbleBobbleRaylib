#include "StateMachineGameStates.h"
#include "EventSystem.h"
#include "entt/entity/fwd.hpp"

void GameplayState::Init() {
    music = &PlayMusic("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");
    StartLevel();

    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::GAMEPLAY | SystemTypeFlags::RENDERING);
}

void GameplayState::StartLevel() {
    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::LEVEL_INSTANTIATION);
    eventSystem.Notify((entt::entity)0, INSTANTIATE_LEVEL, level);
    runner.UpdateSystems();

    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::GAMEPLAY | SystemTypeFlags::RENDERING);
}

void GameplayState::Update() {

    // Move into OnEnter()
    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::GAMEPLAY | SystemTypeFlags::RENDERING);

    runner.UpdateSystems();

    if (eventSystem.ReadEvent(ALL_ENEMIES_DEFEATED).size()) {
        PRINT_INFO("All enemies defeated");
        level++;
        StartLevel();
    }

#ifdef DEBUG_TOOLS
    if (IsKeyPressed(KEY_Z)) {
        EntityFactory::CreateTumblingEnemy(Random::Get().Range(100, 400), 250, Random::Get().GetDirection(), GetItemOfLevel(Random::Get().Range(0, 5)));
    }
    if (IsKeyPressed(KEY_P)) {
        SetTargetFPS(2);
    } else if (IsKeyPressed(KEY_O)) {
        SetTargetFPS(TARGET_FPS);
    }
#endif
}
