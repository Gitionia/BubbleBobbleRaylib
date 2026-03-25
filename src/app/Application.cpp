#include "Application.h"

#include "../ecs/Systems.h"

#include "../behavior/Level.h"
#include "../behavior/Physics.h"

#include "Config.h"
#include "EventSystem.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <string>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

void update(SystemRunner &runner, EventSystem &eventSystem);

#ifdef PLATFORM_WEB
static SystemRunner *systemRunner;
static EventSystem *eventSystem;
void webUpdate() {
    // PRINT_INFO("Updating from PLATFORM_WEB");
    update(*systemRunner);
}
void setupEmscriptionUpdateLoop(SystemRunner &runner, EventSystem &eventSystem) {
    systemRunner = &runner;
    eventSystem = &eventSystem;
    emscripten_set_main_loop(webUpdate, 60, true);
}
#endif

Application::Application(const ApplicationParameters &parameters)
    : window(parameters.width, parameters.height, parameters.title),
      systemRunner(registry, eventSystem) {
    Debug::get().setRegistry(registry);

    window.Init();

    if (!InitAudio()) {
        PRINT_ERROR("Audio Device could not be initialized!");
    }

    Input::Init();

    EntityFactory::get().setRegistry(registry);
    SystemBase::BaseInit();

    Random::Get().Init();

    LoadSprites();
    LoadAnimations();

    systemRunner.Init();
}

Application::~Application() {
    UnloadAllAudio();
    CloseAudioDevice();

    UnloadSprites();
}

void Application::Run() {
    LevelLayout level = LevelLayout::LoadLevel("res/levels/Level2.json");
    EntityFactory::CreateLevel(level);
    setPhysicsColliderData(level);

    Music &music = PlayMusic("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");

    auto dragon = EntityFactory::CreateDragon();

    int points = 0;
    entt::entity textEntity = EntityFactory::CreateModifiableUIText({BP_SIZE(26, 0), 0}, "HI SCORE\n" + std::to_string(points), GREEN);
    ModifiableUITextComponent &ui = registry.get<ModifiableUITextComponent>(textEntity);

#ifdef PLATFORM_WEB
    setupEmscriptionUpdateLoop(systemRunner);
#else
    while (window.IsOpen()) {
        update(systemRunner, eventSystem);

        // should be moved into update() function
        for (auto event : eventSystem.ReadEvent(POINTS_GAINED)) {
            int gainedPoints = *(int *)event.data.get();
            points += gainedPoints;
        }

        ui.text = "HI SCORE\n" + std::to_string(points);
        
        eventSystem.Clear();
    }
#endif
}

void update(SystemRunner &runner, EventSystem& eventSystem) {

// #define PROFILE
#ifdef PROFILE
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif

#ifdef DEBUG_TOOLS

    if (IsKeyPressed(KEY_Z)) {
        EntityFactory::CreateTumblingEnemy(Random::Get().Range(100, 400), 250, Random::Get().GetDirection());
    }

    runner.UpdateSystems();
    UpdateAudio();

    if (IsKeyPressed(KEY_P)) {
        SetTargetFPS(2);
    } else if (IsKeyPressed(KEY_O)) {
        SetTargetFPS(TARGET_FPS);
    }
#endif

#ifdef PROFILE
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    PRINT_INFO("FPS: {}, Frame Time: {}µs", 1000000 / elapsed, elapsed);
#endif
}
