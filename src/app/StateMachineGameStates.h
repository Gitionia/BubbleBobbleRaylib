#pragma once

#include "EventSystem.h"
#include "StateMachine.h"

#include "../level/Level.h"
#include "../level/Physics.h"
#include "../audio/Audio.h"

#include "../ecs/Components.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"

class GameplayState : public StateMachineState {
  public:
    GameplayState(SystemRunner &runner, EventSystem& eventSystem)
        : StateMachineState(runner, eventSystem) {}

    void Init() override;
    void Update() override;

  private:
    LevelLayout level;
    Music *music;
};
