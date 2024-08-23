#include "Application.h"


#include "../ecs/Components.h"
#include "../ecs/Systems.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <raylib.h>

#include <nlohmann/json.hpp>

Application::Application()
	: systemRunner(registry), window(640, 480, "Bubble Bobble")
{
	window.init();
	systemRunner.InitSystems();
}

Application::~Application() = default;

void Application::run()
{
	{
		std::ifstream f("res/levels/Level1.json");
		nlohmann::json data = nlohmann::json::parse(f);
		auto layers = data.find("layers").value();
		auto levelData = layers.at(0).find("data").value();
		std::cout << levelData << std::endl;


		if (levelData.size() != 26 * 28) throw "Invalid Size of leveldata";

		bool* level = new bool[26 * 28];
		for (int i = 0; i < levelData.size(); i++) {
			level[i] = (bool)(int)levelData.at(i);

			if (i % 28 == 0 && i > 0) {
				std::cout << std::endl;
			}
			std::cout << level[i] << ", ";
		}
	}

	Rectangle rect1 {0,0,10,10};
	{
		std::ifstream f("res/MainSpriteSheet.json");
		nlohmann::json data = nlohmann::json::parse(f);

		auto slices = data.find("meta").value().find("slices").value();

		for (const auto& slice : slices) {
			auto name = slice.find("name").value();
			auto bounds = slice.find("keys").value().at(0).find("bounds").value();
			Rectangle rect{ bounds.find("x").value(), bounds.find("y").value(), bounds.find("w").value(), bounds.find("h").value() };
			rect1 = rect;
			std::cout << name << ", x:" << rect.x << ", y:" << rect.y << ", width:" << rect.width << ", height:" << rect.height << std::endl;
		}
	}



	auto createdEntity = registry.create();
	registry.emplace<Position>(createdEntity, 100, 100);
	registry.emplace<Velocity>(createdEntity, 1, 2);
	registry.emplace<BallSize>(createdEntity, 40);
	registry.emplace<Sprite>(createdEntity, rect1);

	createdEntity = registry.create();
	registry.emplace<Position>(createdEntity, 400, 300);
	registry.emplace<Velocity>(createdEntity, 2, -1);
	registry.emplace<BallSize>(createdEntity, 50);
	registry.emplace<Sprite>(createdEntity, rect1);

	createdEntity = registry.create();
	registry.emplace<Position>(createdEntity, 300, 200);
	registry.emplace<Velocity>(createdEntity, -1, 1);
	registry.emplace<BallSize>(createdEntity, 30);
	registry.emplace<Sprite>(createdEntity, rect1);


	while (window.isOpen()) {
		auto view = registry.view<Position, Velocity, BallSize>();
		for (auto entity : view) {
			auto [pos, vel, size] = view.get(entity);

			pos.x += vel.dx;
			pos.y += vel.dy;

			// Check walls collision for bouncing
			if ((pos.x >= (GetScreenWidth() - size.radius)) || (pos.x <= size.radius)) vel.dx *= -1.0f;
			if ((pos.y >= (GetScreenHeight() - size.radius)) || (pos.y <= size.radius)) vel.dy *= -1.0f;

		}

		systemRunner.UpdateSystems();
	}
}
