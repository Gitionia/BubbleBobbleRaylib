#pragma once

#include "entt/entt.hpp"
#include "../graphics/Window.h"

class Application {
public:
	Application();
	~Application();
	void run();

private:
	entt::registry registry;
	Window window;
};