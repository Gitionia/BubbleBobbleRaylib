#include "Application.h"

#include "../ecs/Systems.h"

#include "../level/Level.h"
#include "../level/Physics.h"

#include "Config.h"
#include "EventSystem.h"
#include "StateMachine.h"
#include "StateMachineGameStates.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <string>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

static StateMachine *g_stateMachine;
static EventSystem *g_eventSystem;
void update();

Application::Application(const ApplicationParameters &parameters)
    : window(parameters.width, parameters.height, parameters.title),
      systemRunner(registry, eventSystem),
      stateMachine(systemRunner, new GameplayState(systemRunner, eventSystem)) {
        
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
    stateMachine.Init();

    
    g_stateMachine = &stateMachine;
    g_eventSystem = &eventSystem;
}

Application::~Application() {
    UnloadAllAudio();
    CloseAudioDevice();

    UnloadSprites();
}

void Application::Run() {

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update, 0, true);
    #else
    while (window.IsOpen()) {
        update();
    }
#endif
}

/// Main Loop
void update() {

// #define PROFILE
#ifdef PROFILE
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif

    g_stateMachine->Update();
    UpdateAudio();
    g_eventSystem->Clear();

#ifdef PROFILE
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    PRINT_INFO("FPS: {}, Frame Time: {}µs", 1000000 / elapsed, elapsed);
#endif
}
