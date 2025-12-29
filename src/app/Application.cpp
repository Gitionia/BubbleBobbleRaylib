#include "Application.h"

#include "../ecs/Components.h"
#include "../ecs/Systems.h"

#include "../behavior/Level.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include <raylib.h>

#include <nlohmann/json.hpp>

#include "../behavior/Physics.h"

#include "../utils/Debug.h"
#include "../utils/Log.h"

#include "spdlog/spdlog.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

void update(SystemRunner &runner);

#ifdef PLATFORM_WEB
static SystemRunner *systemRunner;
void webUpdate() {
    // PRINT_INFO("Updating from PLATFORM_WEB");
    update(*systemRunner);
}
void setupEmscriptionUpdateLoop(SystemRunner &runner) {
    systemRunner = &runner;
    emscripten_set_main_loop(webUpdate, 60, true);
}
#endif

Application::Application(const ApplicationParameters &parameters)
    : window(parameters.width, parameters.height, parameters.title),
      systemRunner(registry) {
    Debug::get().setRegistry(registry);

    window.Init();

    if (!InitAudio()) {
        PRINT_ERROR("Audio Device could not be initialized!");
    }

    EntityFactory::get().setRegistry(registry);

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

#ifdef PLATFORM_WEB
    setupEmscriptionUpdateLoop(systemRunner);
#endif

    while (window.IsOpen()) {
        update(systemRunner);
    }
}

void update(SystemRunner &runner) {
    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    runner.UpdateSystems();
    UpdateAudio();

    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // PRINT_INFO("Frame Time: {}µs", std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}
