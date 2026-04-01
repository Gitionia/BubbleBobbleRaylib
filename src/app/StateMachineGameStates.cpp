#include "StateMachineGameStates.h"
#include "EventSystem.h"
#include "StateMachine.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <cstddef>

void GameplayState::OnEnter() {
    music = &PlayMusic("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");

    // creates the level and the game ui
    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::LEVEL_INSTANTIATION | SystemTypeFlags::GAME_UI);
    eventSystem.Notify((entt::entity)0, INSTANTIATE_LEVEL, level);
    eventSystem.Notify((entt::entity)0, INSTANTIATE_GAME_UI, level);
    runner.UpdateSystems();

    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::GAMEPLAY | SystemTypeFlags::RENDERING);
}

void GameplayState::StartLevel() {
    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::LEVEL_INSTANTIATION | SystemTypeFlags::GAME_UI);
    eventSystem.Notify((entt::entity)0, INSTANTIATE_LEVEL, level);
    runner.UpdateSystems();

    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::GAMEPLAY | SystemTypeFlags::RENDERING);
}

bool GameplayState::isStoryLevel() {
    return level == PREVIOUS_TO_BOSS_LEVEL || level == BOSS_LEVEL;
}

std::shared_ptr<StateMachineState> GameplayState::Update() {

    runner.UpdateSystems();

    if (eventSystem.ReadEvent(ALL_ENEMIES_DEFEATED).size()) {
        if (!waitingForCounterToStartNewLevel) {
            counterTillStartNewLevel = FRAME_COUNT_TILL_STARTING_NEW_LEVEL;
            waitingForCounterToStartNewLevel = true;
        }
    }

    // If is story level and story item picked up
    if (isStoryLevel() && false) {
        // handle case
    }

    if (counterTillStartNewLevel > 0) {
        counterTillStartNewLevel--;

    } else if (waitingForCounterToStartNewLevel && counterTillStartNewLevel == 0 && !isStoryLevel()) {
        level++;
        StartLevel();

        waitingForCounterToStartNewLevel = false;
    }

#ifdef DEBUG_TOOLS
    if (Input::IsKeyPressed(KEY_N)) {
        level++;
        StartLevel();
    }
    if (Input::IsKeyPressed(KEY_M)) {
        level--;
        StartLevel();
    }
    if (Input::IsKeyPressed(KEY_Q)) {
        level = 101;
        StartLevel();

    } else if (Input::IsKeyPressed(KEY_W)) {
        level = 1;
        StartLevel();

    } else if (Input::IsKeyPressed(KEY_E)) {
        StartLevel();
    }

    if (Input::IsKeyPressed(KEY_P)) {
        SetTargetFPS(2);

    } else if (Input::IsKeyPressed(KEY_I)) {
        SetTargetFPS(600);

    } else if (Input::IsKeyPressed(KEY_O)) {
        SetTargetFPS(TARGET_FPS);
    }
    if (Input::IsKeyPressed(KEY_F)) {
        ToggleFullscreen();
    }
    if (Input::IsKeyPressed(KEY_B)) {
        ToggleBorderlessWindowed();
    }
#endif

    return nullptr;
}

void TitleScreenState::OnEnter() {
    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::RENDERING | SystemTypeFlags::TITLE_SCREEN);
    // Play Music
}

std::shared_ptr<StateMachineState> TitleScreenState::Update() {
#ifdef NDEBUG
    bool titleScreenSkip = false;
#else
    bool titleScreenSkip = true;
#endif

    if (titleScreenSkip || Input::IsKeyDown(Key::Any)) {
        // Cleans up Title Screen Entities
        eventSystem.Notify((entt::entity)0, START_GAMEPLAY, 0);
        runner.UpdateSystems();

        return std::make_shared<GameplayState>(runner, eventSystem);

    } else {
        runner.UpdateSystems();

        return nullptr;
    }
}
