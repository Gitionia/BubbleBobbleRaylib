#pragma once

#include "../ecs/Systems.h"
#include "EventSystem.h"
#include "entt/entity/fwd.hpp"
#include <memory>

class StateMachineState {
  public:
    StateMachineState(SystemRunner& runner, EventSystem& eventSystem) :
      runner(runner), eventSystem(eventSystem) {}

    virtual void OnEnter() {}
    virtual std::shared_ptr<StateMachineState> Update() = 0;
    virtual void OnExit() {}

  protected:
    SystemRunner& runner;
    EventSystem& eventSystem;
};

class StateMachine {
  public:
    StateMachine(SystemRunner &runner, std::shared_ptr<StateMachineState> firstState);
    ~StateMachine() = default;

    void Update();

  private:
    std::shared_ptr<StateMachineState> currentState;
    bool enterNewState = true;
};
