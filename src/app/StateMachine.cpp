#include "StateMachine.h"

StateMachine::StateMachine(SystemRunner &runner, StateMachineState* firstState) 
    : currentState(firstState) { }

void StateMachine::Update() {
    currentState->Update();
}
