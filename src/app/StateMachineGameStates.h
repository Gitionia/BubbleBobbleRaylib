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
    Music *music;
    int level = 7;

    bool waitingForCounterToStartNewLevel = false;
    int counterTillStartNewLevel = 0;
    static constexpr int FRAME_COUNT_TILL_STARTING_NEW_LEVEL = 5 * TARGET_FPS;
};

class TitleScreenState : public StateMachineState {
  public:
    TitleScreenState(SystemRunner &runner, EventSystem &eventSystem)
        : StateMachineState(runner, eventSystem) {}

    void OnEnter() override;
    std::shared_ptr<StateMachineState> Update() override;
};
