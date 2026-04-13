#pragma once

#include "../audio/Audio.h"
#include "../ecs/EntityFactory.h"
#include "../ecs/Systems.h"
#include "../graphics/Sprites.h"
#include "../graphics/Window.h"
#include "../utils/Recorder.h"
#include "Config.h"
#include "EventSystem.h"
#include "StateMachine.h"
#include "WindowConfig.h"

struct ApplicationParameters {
    int width = TARGET_WINDOW_WIDTH;
    int height = TARGET_WINDOW_HEIGHT;
    const char *title = "Bubble Bobble";
    std::string recordedFilePath = "";
    int level = 1;

    bool printHelp = false;
    bool runApp = true;
};

ApplicationParameters ParseApplicationParametersAndPrintErrors(int argc, char **argv);

class Application {
  public:
    explicit Application(const ApplicationParameters &parameters);
    ~Application();
    void Run();

  private:
    std::shared_ptr<spdlog::logger> stdoutLogger;
    std::shared_ptr<spdlog::logger> fileLogger;

    Window window;
    EventSystem eventSystem;
    entt::registry registry{};
    SystemRunner systemRunner;
    StateMachine stateMachine;
    std::shared_ptr<InputSimulator> inputSimulator;
};
