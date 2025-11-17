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
	Animations::Get().LoadAnimations(spriteManager);
}

Application::~Application() {
	audio.Close();
}

void Application::Run()
{
	LevelLayout level = LoadLevel("res/levels/Level2.json");
	factory.CreateLevel(level);

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
