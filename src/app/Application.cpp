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

#include "../behavior/Physics.h"

#include "../utils/Debug.h"

Application::Application(const ApplicationParameters& parameters)
	: window(parameters.width, parameters.height, parameters.title), 
    systemRunner(registry)
{
	Debug::get().setRegistry(registry);

	window.Init();

	if (!InitAudio()) {
		std::printf("Error: Audio Device could not be initialized!");
	}

	EntityFactory::get().setRegistry(registry);

	LoadSprites();
	LoadAnimations();

	systemRunner.Init();
}

Application::~Application() {
	UnloadAllAudio();
	CloseAudioDevice();

	UnloadSprites();
}

void Application::Run()
{
	LevelLayout level = LevelLayout::LoadLevel("res/levels/Level2.json");
	EntityFactory::CreateLevel(level);
	setPhysicsColliderData(level);

	Music& music = PlayMusic("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");


	auto dragon = EntityFactory::CreateDragon();


#ifdef _DEBUG
bool slowMotion = false;
#endif


	while (window.IsOpen()) {
		systemRunner.UpdateSystems();
		UpdateAudio();

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
