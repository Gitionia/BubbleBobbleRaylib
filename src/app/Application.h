#pragma once

#include "../ecs/Systems.h"
#include "entt/entt.hpp"
#include "../graphics/Window.h"

struct ApplicationParameters {
	int width = 640;
	int height = 480;
	const char* title = "Bubble Bobble";
};

class Application {
public:
	explicit Application(const ApplicationParameters& parameters);
	~Application();
	void run();

private:
	Window window;
	entt::registry registry;
	SystemRunner systemRunner;
};