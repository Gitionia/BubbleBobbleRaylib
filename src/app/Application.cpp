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
    factory(registry, spriteManager),
    systemRunner(registry, factory, spriteManager)
{
	if (!AudioPlayer::Get().Init()) {
		std::printf("Error: Audio Device could not be initialized!");
	}

	Debug::Get().setRegistry(registry);
	window.Init();
	spriteManager.LoadSprites();
	systemRunner.Init();
	Animations::Get().LoadAnimations(spriteManager);
}

Application::~Application() {
	AudioPlayer::Get().Close();
}

void Application::Run()
{
	LevelLayout level = LevelLayout::LoadLevel("res/levels/Level2.json");
	factory.CreateLevel(level);
	setPhysicsColliderData(level);

	Music& music = AudioPlayer::Get().PlayMusic("res/sounds/tim-follin-atari/02 Bubble Bobble - Ingame-Title__Loop.mp3");


	auto dragon = factory.CreateDragon();


#ifdef _DEBUG
bool slowMotion = false;
#endif


	while (window.IsOpen()) {
		systemRunner.UpdateSystems();
		AudioPlayer::Get().Update();

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
