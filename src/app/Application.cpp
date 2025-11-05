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
	: window(parameters.width, parameters.height, parameters.title), 
    factory(registry, spriteManager),
    systemRunner(registry, factory, spriteManager, audio)
{
	if (!audio.Init()) {
		std::printf("Error: Audio Device could not be initialized!");
	}

	window.Init();
	spriteManager.LoadSprites();
	systemRunner.Init();
}

Application::~Application() {
	audio.Close();
}

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

	auto music = LoadMusicStream("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");
	PlayMusicStream(music);

	auto dragon = factory.CreateDragon();


#ifdef _DEBUG
bool slowMotion = false;
#endif


	while (window.IsOpen()) {
		systemRunner.UpdateSystems();

		UpdateMusicStream(music);

		// float playbackTime = GetMusicTimePlayed(music);
		// if (playbackTime > 46.1f) {
		// 	StopMusicStream(music);
		// 	PlayMusicStream(music);
		// }

#ifdef _DEBUG
        
        if (IsKeyDown(KEY_P)) {
            // slowMotion = !slowMotion;
            // window.SetFPS(slowMotion ? 1 : TARGET_FPS);
            window.SetFPS(1);
        } 
        else 
            window.SetFPS(60);
#endif
	}
}
