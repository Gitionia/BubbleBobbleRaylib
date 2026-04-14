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
    GameplayState(SystemRunner &runner, EventSystem &eventSystem, int level)
        : StateMachineState(runner, eventSystem), level(level) {}

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

class VictorySceneState : public StateMachineState {
  public:
    VictorySceneState(SystemRunner &runner, EventSystem &eventSystem)
        : StateMachineState(runner, eventSystem) {}

    void OnEnter() override;
    std::shared_ptr<StateMachineState> Update() override;

  private:
    int counter = 0;
    constexpr static int COUNTER_TILL_CAN_EXIT = 60 * 10;
};

class IntroSceneState : public StateMachineState {
  public:
    IntroSceneState(SystemRunner &runner, EventSystem &eventSystem, int level)
        : StateMachineState(runner, eventSystem), level(level) {}

    void OnEnter() override;
    std::shared_ptr<StateMachineState> Update() override;

  private:
    int level;
};

class TitleScreenState : public StateMachineState {
  public:
    TitleScreenState(SystemRunner &runner, EventSystem &eventSystem, int level = 1)
        : StateMachineState(runner, eventSystem), level(level) {}

    void OnEnter() override;
    std::shared_ptr<StateMachineState> Update() override;

  private:
    int level;
};
