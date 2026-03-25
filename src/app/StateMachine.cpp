#include "StateMachine.h"

StateMachine::StateMachine(SystemRunner &runner, StateMachineState* firstState) 
    : currentState(firstState) { }

void StateMachine::Init() {
    currentState->Init();
}

void StateMachine::Update() {
    currentState->Update();
}
