#include "Sprites.h"

#include "raylib.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"

SpriteManager::SpriteManager()
	: mainSpriteSheet(), spriteMap() {
}

SpriteManager::~SpriteManager()
{
	UnloadTexture(mainSpriteSheet);
}

void SpriteManager::LoadSprites()
{
	mainSpriteSheet = LoadTexture("res/MainSpriteSheet.png");


	Rectangle rect1 {0,0,10,10};
	std::ifstream f("res/MainSpriteSheet.json");
	nlohmann::json data = nlohmann::json::parse(f);

	auto slices = data.find("meta").value().find("slices").value();

	for (const auto& slice : slices) {
		auto name = slice.find("name").value();
		auto bounds = slice.find("keys").value().at(0).find("bounds").value();
		Rectangle rect{ bounds.find("x").value(), bounds.find("y").value(), bounds.find("w").value(), bounds.find("h").value() };
		rect1 = rect;
		Sprite sprite = { mainSpriteSheet, rect};
		spriteMap.insert({name, sprite});
		std::cout << name << ", x:" << rect.x << ", y:" << rect.y << ", width:" << rect.width << ", height:" << rect.height << std::endl;
	}
}

Sprite& SpriteManager::GetSprite(const std::string& name) {
	return spriteMap.at(name);
}

