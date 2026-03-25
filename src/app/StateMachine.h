#pragma once

#include "../ecs/Systems.h"
#include "EventSystem.h"
#include "entt/entity/fwd.hpp"

class StateMachineState {
  public:
    StateMachineState(SystemRunner& runner) :
      runner(runner) {}

    virtual void OnEnter() {}
    virtual void Update() = 0;
    virtual void OnExit() {}

  protected:
    SystemRunner& runner;
};

class StateMachine {
  public:
    StateMachine(SystemRunner &runner, StateMachineState* firstState);
    ~StateMachine() = default;

    void Update();

  private:
    StateMachineState *currentState;
};
