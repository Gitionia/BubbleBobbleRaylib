#include "StateMachineGameStates.h"
#include "EventSystem.h"

void GameplayState::Init() {
    level = LevelLayout::LoadLevel("res/levels/Level2.json");
    EntityFactory::CreateLevel(level);
    setPhysicsColliderData(level);
    music = &PlayMusic("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");

    auto dragon = EntityFactory::CreateDragon();
}

void GameplayState::Update() {
    
    // Move into OnEnter()
    runner.OnlyHaveSystemsEnabledThatMatchAnyFlag(SystemTypeFlags::GAMEPLAY | SystemTypeFlags::RENDERING);

    runner.UpdateSystems();

    if (eventSystem.ReadEvent(ALL_ENEMIES_DEFEATED).size()) {
        EntityFactory::CreateLevel(level);
        
        setPhysicsColliderData(level);
    }


#ifdef DEBUG_TOOLS
    if (IsKeyPressed(KEY_Z)) {
        EntityFactory::CreateTumblingEnemy(Random::Get().Range(100, 400), 250, Random::Get().GetDirection(), GetItemOfLevel(Random::Get().Range(0, 5)));
    }
    if (IsKeyPressed(KEY_P)) {
        SetTargetFPS(2);
    } else if (IsKeyPressed(KEY_O)) {
        SetTargetFPS(TARGET_FPS);
    }
#endif
}
