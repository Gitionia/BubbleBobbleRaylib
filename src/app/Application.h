#pragma once
#include <iostream>

#include "../ecs/Systems.h"
#include "entt/entt.hpp"
#include "../graphics/Window.h"

class Application {
public:
	Application();
	~Application();
	void run();

private:
	Window window;
	entt::registry registry;
	SystemRunner systemRunner;
};