#pragma once

#include "../audio/Audio.h"
#include "../ecs/EntityFactory.h"
#include "../ecs/Systems.h"
#include "../graphics/Sprites.h"
#include "../graphics/Window.h"
#include "Config.h"
#include "WindowConfig.h"
#include "EventSystem.h"
#include "StateMachine.h"

struct ApplicationParameters {
    int width = TARGET_WINDOW_WIDTH;
    int height = TARGET_WINDOW_HEIGHT;
    const char *title = "Bubble Bobble";
};

class Application {
  public:
    explicit Application(const ApplicationParameters &parameters);
    ~Application();
    void Run();

  private:
    Window window;
    EventSystem eventSystem;
    entt::registry registry{};
    SystemRunner systemRunner;
    StateMachine stateMachine;

    std::shared_ptr<spdlog::logger> stdoutLogger;
    std::shared_ptr<spdlog::logger> fileLogger;
};
