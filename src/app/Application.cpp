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
	: window(parameters.width, parameters.height, parameters.title), systemRunner(registry, spriteManager)
{
	window.Init();
	spriteManager.LoadSprites();
}

Application::~Application() = default;

void createTile(entt::registry& registry, const SpriteManager& spriteManager, int x, int y, int tileSize) {
	auto createdEntity = registry.create();
	registry.emplace<Position>(createdEntity, x * UNITS_PER_BLOCK, y * UNITS_PER_BLOCK);
	RenderData data = {spriteManager.GetSpriteHandle("Level1"), {1, 1}};
	registry.emplace<RenderData>(createdEntity, data);
	registry.emplace<LevelTileTag>(createdEntity);
	registry.emplace<Collider>(createdEntity, UNITS_PER_BLOCK, UNITS_PER_BLOCK, 0, 0);
}

void Application::Run()
{
	constexpr int tileSize = 16;

	LevelLayout level = LoadLevel("res/levels/Level2.json");

	for (int x = 2; x < LevelLayout::WIDTH + 2; ++x) {
		for (int y = 0; y < LevelLayout::HEIGHT; ++y) {
			if (level.Get(x - 2, y)) {
				createTile(registry, spriteManager, x, y, tileSize);
			}
		}
	}

	int rows[] = {0, 1, 30, 31};
	for (int x : rows) {
		for (int y = 0; y < LevelLayout::HEIGHT; ++y) {
			// Here tile creation could be more leightweight (e.g. remove Collider)
			createTile(registry, spriteManager, x, y, tileSize);
		}
	}


	auto dragon = registry.create();
	registry.emplace<Position>(dragon, 5 * UNITS_PER_BLOCK, 5 * UNITS_PER_BLOCK);
	registry.emplace<RenderData>(dragon, RenderData(spriteManager.GetSpriteHandle("Dragon-Idle-1"), {2, 2}));
	registry.emplace<Collider>(dragon, 2 * UNITS_PER_BLOCK, UNITS_PER_BLOCK, 0, UNITS_PER_BLOCK);
    // Spikes on the back have full height and 6 pixels to the end of the sprite
	registry.emplace<DragonSpikeCollider>(dragon, 6 * UNITS_PER_PIXEL, UNITS_PER_BLOCK, UNITS_PER_BLOCK - 6 * UNITS_PER_PIXEL, 0);
	registry.emplace<WalkingActorComponent>(dragon);
	registry.emplace<DragonTag>(dragon);




	while (window.IsOpen()) {
		systemRunner.UpdateSystems();
	}
}
