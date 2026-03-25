#pragma once

#include "StateMachine.h"

class GameplayState : public StateMachineState {
  public:
    GameplayState(SystemRunner &runner)
        : StateMachineState(runner) {}

    void Init() override;
    void Update() override;

  private:
    LevelLayout level;
    Music *music;
    int points;
    entt::entity textEntity;
    ModifiableUITextComponent *ui;
};
