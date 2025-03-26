#include "Application.h"


#include "../ecs/Components.h"
#include "../ecs/Systems.h"

#include "../behavior/Level.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <raylib.h>

#include <nlohmann/json.hpp>

Application::Application(const ApplicationParameters& parameters)
	: window(parameters.width, parameters.height, parameters.title), systemRunner(registry)
{
	window.Init();
	spriteManager.LoadSprites();
}

Application::~Application() = default;

void Application::Run()
{
	constexpr int tileSize = 16;

	LevelLayout level = LoadLevel("res/levels/Level1.json");

	for (int x = 0; x < LevelLayout::WIDTH; ++x) {
		for (int y = 0; y < LevelLayout::HEIGHT; ++y) {
			if (level.Get(x, y)) {
				auto createdEntity = registry.create();
				registry.emplace<Position>(createdEntity, x * tileSize, y * tileSize);
				RenderData data = {spriteManager.GetSprite("Level1"), {1, 1}};
				registry.emplace<RenderData>(createdEntity, data);
				registry.emplace<LevelTileTag>(createdEntity);
				registry.emplace<Collider>(createdEntity, 16, 16, 0, 0);
			}
		}
	}


	auto dragon = registry.create();
	registry.emplace<Position>(dragon, 100, 100);
	registry.emplace<DragonComponent>(dragon, 0);
	registry.emplace<RenderData>(dragon, RenderData(spriteManager.GetSprite("Dragon-Idle-1"), {2, 2}));
	registry.emplace<Collider>(dragon, 32, 16, 0, 16);
	registry.emplace<DragonTag>(dragon);




	while (window.IsOpen()) {
		systemRunner.UpdateSystems();
	}
}
