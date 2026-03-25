#pragma once

#include "StateMachine.h"

class GameplayState : public  StateMachineState {
    public:
    GameplayState(SystemRunner& runner) 
        : StateMachineState(runner) {}

    
    void Update() override;
};
