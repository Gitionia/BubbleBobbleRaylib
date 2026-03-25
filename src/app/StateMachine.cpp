#include "StateMachine.h"

#include "../level/Level.h"
#include "../level/Physics.h"

#include "../audio/Audio.h"
#include "StateMachine.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <string>

StateMachine::StateMachine(SystemRunner &runner) 
    : runner(runner) { }

void StateMachine::Update() {
    static LevelLayout level;
    static bool firstFrame = true;
    static Music *music;
    static int points;
    static entt::entity textEntity;
    static ModifiableUITextComponent *ui;
    if (firstFrame) {
        firstFrame = false;
        level = LevelLayout::LoadLevel("res/levels/Level2.json");
        EntityFactory::CreateLevel(level);
        setPhysicsColliderData(level);
        music = &PlayMusic("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");

        auto dragon = EntityFactory::CreateDragon();

        // points = 0;
        // textEntity = EntityFactory::CreateModifiableUIText({BP_SIZE(26, 0), 0}, "HI SCORE\n" + std::to_string(points), GREEN);

        // ui = &registry.get<ModifiableUITextComponent>(textEntity);
    }

    // // should be moved into update() function
    // for (const Event &event : eventSystem.ReadEvent(POINTS_GAINED)) {
    //     int gainedPoints = event.data;
    //     points += gainedPoints;
    // }

    // ui.text = "HI SCORE\n" + std::to_string(points);

    runner.UpdateSystems();

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
