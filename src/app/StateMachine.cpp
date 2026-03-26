#include "StateMachine.h"

StateMachine::StateMachine(SystemRunner &runner, std::shared_ptr<StateMachineState> firstState) 
    : currentState(firstState) { }

void StateMachine::Init() {
    currentState->Init();
}

void StateMachine::Update() {

    if (enterNewState) {
        currentState->OnEnter();
        enterNewState = false;
    }

    std::shared_ptr<StateMachineState> newState = currentState->Update();

    if (newState) {
        currentState->OnExit();
        currentState = newState;
        enterNewState = true;
    }
}
