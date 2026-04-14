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

static InputSimulator::Mode inputSimulationModeChooser(const std::string &path) {
#ifdef NDEBUG
    return InputSimulator::NO_RECORD;
#else
    if (path.empty()) {
        return InputSimulator::RECORD;
    } else {
        return InputSimulator::REPLAY;
    }
#endif
}

static std::string inputRecorderFileChooser(const std::string &path) {
#ifdef NDEBUG
    return "";
#else
    if (path.empty()) {
        return std::format("./recordedInput/log-{}.input", GetCurrentTimeStamp());
    } else {
        return path;
    }
#endif
}

Application::Application(const ApplicationParameters &parameters)
    : stdoutLogger(spdlog::stdout_color_mt("console")),
      fileLogger(spdlog::basic_logger_mt("file", "logs/log.txt")),
      window(parameters.width, parameters.height, parameters.title),
      systemRunner(registry, eventSystem),
      stateMachine(systemRunner, std::make_shared<VictorySceneState>(systemRunner, eventSystem)),
      inputSimulator(std::make_shared<InputSimulator>(inputSimulationModeChooser(parameters.recordedFilePath), inputRecorderFileChooser(parameters.recordedFilePath))) {

#ifdef NDEBUG
    auto logLevel = spdlog::level::err;
#else
    auto logLevel = spdlog::level::info;
#endif
    stdoutLogger->set_level(logLevel);
    fileLogger->set_level(logLevel);

    PRINT_INFO("Starting game");

    Debug::get().setRegistry(registry);

    window.Init();

    if (!InitAudio()) {
        PRINT_CRITICAL("Audio Device could not be initialized! Trying to run without Audio");
    }

    Input::Init(inputSimulator);

    EntityFactory::get().setRegistry(registry);
    SystemBase::BaseInit();

    Random::Get().Init();

    LoadSprites();
    LoadAnimations();
    LoadAudio();

    systemRunner.Init();

    g_stateMachine = &stateMachine;
    g_eventSystem = &eventSystem;
}

Application::~Application() {
    CleanUpAudio();
    CloseAudioDevice();

    UnloadSprites();

    inputSimulator->SaveRecording();
}

void Application::Run() {

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update, 0, true);
#else
    while (window.IsOpen()) {
        inputSimulator->Update();
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

    Input::UpdateTouchInput();
    g_stateMachine->Update();
    UpdateAudio();
    g_eventSystem->Clear();

#ifdef PROFILE
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    PRINT_INFO("FPS: {}, Frame Time: {}µs", 1000000 / elapsed, elapsed);
#endif
}

static bool nextArg(int &argc, char **&argv) {
    if (argc > 0) {
        argc--;
        argv++;
    }

    return argc > 0;
}

ApplicationParameters ParseApplicationParametersAndPrintErrors(int argc, char **argv) {
    ApplicationParameters params;

    nextArg(argc, argv);
    while (argc > 0) {
        std::string current = argv[0];

        if (current == "--levelFile") {
            nextArg(argc, argv);
            if (argc > 0) {
                std::string levelStr = argv[0];

                if (levelStr.size() < 13) {
                    printf("argument %s should be like path/to/LevelXYZ.json\n", levelStr.c_str());
                    params.runApp = false;
                    params.printHelp = true;
                    break;
                }
                // LevelXYZ.json
                levelStr = levelStr.substr(levelStr.size() - 8, 3);
                if (!hasStringOnlyDigits(levelStr)) {
                    printf("argument %s should be like path/to/LevelXYZ.json\n", levelStr.c_str());
                    params.runApp = false;
                    params.printHelp = true;
                    break;
                }
                int levelNr = std::stoi(levelStr);

                params.level = levelNr;
            }

        } else if (current == "--levelNr") {
            nextArg(argc, argv);
            if (argc > 0) {
                std::string levelStr = argv[0];
                if (!hasStringOnlyDigits(levelStr)) {
                    printf("argument %s should be number between 001 and 999\n", levelStr.c_str());
                    params.runApp = false;
                    params.printHelp = true;
                    break;
                }

                int levelNr = std::stoi(levelStr);
                params.level = levelNr;
            }

        } else if (current == "--playRecording") {
            nextArg(argc, argv);
            if (argc > 0) {
                std::string recordingPath = argv[0];

                params.recordedFilePath = recordingPath;
            }

        } else if (current == "--help") {
            params.runApp = false;
            params.printHelp = true;

        } else {
            printf("Could not parse arg %s\n", argv[0]);
            params.runApp = false;
            params.printHelp = true;
        }

        nextArg(argc, argv);
    }

    return params;
}
