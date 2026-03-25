#pragma once

#include "../ecs/Systems.h"
#include "EventSystem.h"
#include "entt/entity/fwd.hpp"

class StateMachineState {
  public:
    StateMachineState(SystemRunner& runner, EventSystem& eventSystem) :
      runner(runner), eventSystem(eventSystem) {}

    virtual void Init() {}
    virtual void OnEnter() {}
    virtual void Update() = 0;
    virtual void OnExit() {}

  protected:
    SystemRunner& runner;
    EventSystem& eventSystem;
};

class StateMachine {
  public:
    StateMachine(SystemRunner &runner, StateMachineState* firstState);
    ~StateMachine() = default;

    void Init();
    void Update();

  private:
    StateMachineState *currentState;
};
