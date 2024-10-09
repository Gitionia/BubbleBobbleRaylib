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

void Application::run()
{
	constexpr int tileSize = 16;

	{
		std::ifstream f("res/levels/Level1.json");
		nlohmann::json data = nlohmann::json::parse(f);
		auto layers = data.find("layers").value();
		auto levelData = layers.at(0).find("data").value();
		std::cout << levelData << std::endl;


		if (levelData.size() != 26 * 28) throw "Invalid Size of leveldata";

		LevelLayout level;
		for (int i = 0; i < levelData.size(); i++) {
			level.SetTile(i, (bool)(int)levelData.at(i));
		}

		for (int x = 0; x < LevelLayout::WIDTH; ++x) {
			for (int y = 0; y < LevelLayout::HEIGHT; ++y) {
				if (level.Get(x, y)) {
					auto createdEntity = registry.create();
					registry.emplace<Position>(createdEntity, x * tileSize, y * tileSize);
					registry.emplace<RenderData>(createdEntity, spriteManager.GetSprite("Dragon-Idle-1"));
				}
			}
		}
	}

	auto createdEntity = registry.create();
	registry.emplace<Position>(createdEntity, 100, 100);
	registry.emplace<Velocity>(createdEntity, 1, 2);
	registry.emplace<BallSize>(createdEntity, 40);
	registry.emplace<RenderData>(createdEntity, spriteManager.GetSprite("Dragon-Idle-1"));

	createdEntity = registry.create();
	registry.emplace<Position>(createdEntity, 400, 300);
	registry.emplace<Velocity>(createdEntity, 2, -1);
	registry.emplace<BallSize>(createdEntity, 50);
	registry.emplace<RenderData>(createdEntity, spriteManager.GetSprite("Dragon-Idle-1"));

	createdEntity = registry.create();
	registry.emplace<Position>(createdEntity, 300, 200);
	registry.emplace<Velocity>(createdEntity, -1, 1);
	registry.emplace<BallSize>(createdEntity, 30);
	registry.emplace<RenderData>(createdEntity, spriteManager.GetSprite("Dragon-Idle-1"));





	while (window.IsOpen()) {
		systemRunner.UpdateSystems();
	}
}
