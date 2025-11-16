#include "Sprites.h"

#include "raylib.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"


SpriteManager::SpriteManager()
	: mainSpriteSheet(), levelTilesSpriteSheet()
{
}

SpriteManager::~SpriteManager()
{
	UnloadTexture(mainSpriteSheet);
	UnloadTexture(levelTilesSpriteSheet);
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

void SpriteManager::addSpritesToSpriteMap(const Texture2D& spriteSheet, const std::string& sliceInformationFilepath) {
	std::ifstream f(sliceInformationFilepath);
	nlohmann::json data = nlohmann::json::parse(f);

	auto slices = data.find("meta").value().find("slices").value();

	for (const auto& slice : slices) {
		auto name = slice.find("name").value();
		auto key = slice.find("keys").value().at(0);
		auto bounds = key.find("bounds").value();
		int xOffset = 0;
		int yOffset = 0;
		if (key.contains("pivot")) {
			auto offset = key.find("pivot").value();
			xOffset = offset.find("x").value();
			yOffset = offset.find("y").value();
		}

		Rectangle rect{ bounds.find("x").value(), bounds.find("y").value(), bounds.find("w").value(), bounds.find("h").value() };
		sprites.emplace_back(&spriteSheet, rect, xOffset, yOffset);
		spriteMap.insert({name, (SpriteHandle)(sprites.size() - 1)});
	}
}

SpriteHandle SpriteManager::GetSpriteHandle(const std::string &name) const {
	return spriteMap.at(name);
}

SpriteHandle SpriteManager::GetSpriteHandleChecked(const std::string &name) const {
	if (spriteMap.find(name) == std::end(spriteMap)) {
		return -1;
	}
	return GetSpriteHandle(name);
}

const Sprite & SpriteManager::GetSprite(SpriteHandle handle) const {
	return sprites.at(handle);
}
