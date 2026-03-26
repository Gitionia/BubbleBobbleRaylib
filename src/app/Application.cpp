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
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
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
      stateMachine(systemRunner, std::make_shared<TitleScreenState>(systemRunner, eventSystem)) {

#ifdef NDEBUG
    auto logLevel = spdlog::level::error;
#else
    auto logLevel = spdlog::level::info;
#endif
    stdoutLogger = spdlog::stdout_color_mt("console");
    fileLogger = spdlog::basic_logger_mt("file", "logs/log.txt");
    stdoutLogger->set_level(logLevel);
    fileLogger->set_level(logLevel);

    PRINT_INFO("Starting game");

    Debug::get().setRegistry(registry);

    window.Init();

    if (!InitAudio()) {
        PRINT_CRITICAL("Audio Device could not be initialized! Trying to run without Audio");
    }

    Input::Init();

    EntityFactory::get().setRegistry(registry);
    SystemBase::BaseInit();

    Random::Get().Init();

    LoadSprites();
    LoadAnimations();

    systemRunner.Init();
    

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
