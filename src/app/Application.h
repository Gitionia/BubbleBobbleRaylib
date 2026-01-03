#pragma once

#include "../audio/Audio.h"
#include "../ecs/EntityFactory.h"
#include "../ecs/Systems.h"
#include "../graphics/Sprites.h"
#include "../graphics/Window.h"
#include "Config.h"
#include "entt/entt.hpp"

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
    entt::registry registry{};
    SystemRunner systemRunner;
};
