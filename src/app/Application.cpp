#include "Application.h"

#include "../ecs/Systems.h"

#include "../behavior/Level.h"
#include "../behavior/Physics.h"

#include "Config.h"
#include "raylib.h"

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


#ifdef PLATFORM_WEB
    setupEmscriptionUpdateLoop(systemRunner);
#else
    while (window.IsOpen()) {
        update(systemRunner);
    }
#endif

}

void update(SystemRunner &runner) {

// #define PROFILE
#ifdef PROFILE
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif

    if (IsKeyPressed(KEY_Z)) {
        printf("Hello World");
        EntityFactory::CreateTumblingEnemy(100, 250, 1);
    }

    runner.UpdateSystems();
    UpdateAudio();

#ifdef _DEBUG
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
