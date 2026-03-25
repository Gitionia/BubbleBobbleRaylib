#pragma once

#include "EventSystem.h"
#include "entt/entity/fwd.hpp"
#include "../ecs/Systems.h"

class StateMachine {
public:
    StateMachine(SystemRunner& runner);
    ~StateMachine() = default;

    void Update();

private:
    SystemRunner& runner;
};

