#pragma once

#include "../audio/Audio.h"
#include "../ecs/Systems.h"
#include "entt/entt.hpp"
#include "../ecs/EntityFactory.h"
#include "../graphics/Window.h"
#include "../graphics/Sprites.h"

struct ApplicationParameters {
	int width = 32*32;
	int height = 32*26;
	const char* title = "Bubble Bobble";
};

class Application {
public:
	explicit Application(const ApplicationParameters& parameters);
	~Application();
	void Run();

private:
	Window window;
	SpriteManager spriteManager{};
    entt::registry registry{};
    EntityFactory factory;
	SystemRunner systemRunner;
};