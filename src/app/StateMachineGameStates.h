#pragma once

#include "Config.h"
#include "EventSystem.h"
#include "StateMachine.h"

#include "../audio/Audio.h"
#include "../level/Level.h"
#include "../level/Physics.h"

#include "../ecs/Components.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"

class GameplayState : public StateMachineState {
  public:
    GameplayState(SystemRunner &runner, EventSystem &eventSystem)
        : StateMachineState(runner, eventSystem) {}

    void OnEnter() override;
    std::shared_ptr<StateMachineState> Update() override;

  private:
    void StartLevel();

    private:
    const Music *mainThemeMusic;
    int level = 1;
    static constexpr int BOSS_LEVEL = 60;
    static constexpr int PREVIOUS_TO_BOSS_LEVEL = BOSS_LEVEL - 1;
    bool isStoryLevel();

    bool waitingForCounterToStartNewLevel = false;
    int counterTillStartNewLevel = 0;
    static constexpr int FRAME_COUNT_TILL_STARTING_NEW_LEVEL = 5 * TARGET_FPS;

    int playerCount = 1;
  };

class IntroSceneState : public StateMachineState {
    public:
    IntroSceneState(SystemRunner &runner, EventSystem &eventSystem)
        : StateMachineState(runner, eventSystem) {}

    void OnEnter() override;
    std::shared_ptr<StateMachineState> Update() override;

};

class TitleScreenState : public StateMachineState {
  public:
    TitleScreenState(SystemRunner &runner, EventSystem &eventSystem)
        : StateMachineState(runner, eventSystem) {}

    void OnEnter() override;
    std::shared_ptr<StateMachineState> Update() override;
};
