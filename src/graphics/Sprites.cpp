#include "Sprites.h"

#include "raylib.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"

SpriteManager::SpriteManager()
	: mainSpriteSheet(), levelTilesSpriteSheet(), spriteMap()
{
}

SpriteManager::~SpriteManager()
{
	UnloadTexture(mainSpriteSheet);
}

/**
 * Needs to be called before GetSprite(...) can be used.
 * Loads sprites from sprite sheets into spritemap.
 */
void SpriteManager::LoadSprites()
{
	mainSpriteSheet = LoadTexture("res/sprites/MainSpriteSheet.png");
	addSpritesToSpriteMap(mainSpriteSheet, "res/sprites/MainSpriteSheet.json");

	levelTilesSpriteSheet = LoadTexture("res/sprites/LevelTiles.png");
	addSpritesToSpriteMap(levelTilesSpriteSheet, "res/sprites/LevelTiles.json");
}

void SpriteManager::addSpritesToSpriteMap(Texture2D& spriteSheet, const std::string& sliceInformationFilepath) {
	std::ifstream f(sliceInformationFilepath);
	nlohmann::json data = nlohmann::json::parse(f);

	auto slices = data.find("meta").value().find("slices").value();

	for (const auto& slice : slices) {
		auto name = slice.find("name").value();
		auto bounds = slice.find("keys").value().at(0).find("bounds").value();

		Rectangle rect{ bounds.find("x").value(), bounds.find("y").value(), bounds.find("w").value(), bounds.find("h").value() };
		Sprite sprite = { spriteSheet, rect};
		spriteMap.insert({name, sprite});
	}
}

Sprite& SpriteManager::GetSprite(const std::string& name) {
	return spriteMap.at(name);
}

