#include "Sprites.h"

#include "raylib.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"



SpriteManager::~SpriteManager()
{
	for (auto& texture : textures) {
		UnloadTexture(texture);
	}
}

/**
 * Needs to be called before GetSprite(...) can be used.
 * Loads sprites from sprite sheets into spritemap.
 */
void SpriteManager::LoadSprites()
{
	Texture2D mainSpriteSheet = LoadTexture("res/sprites/MainSpriteSheet.png");
	addSpriteSheetToSpriteMap(mainSpriteSheet, "res/sprites/MainSpriteSheet.json");

	Texture2D levelTilesSpriteSheet = LoadTexture("res/sprites/LevelTiles.png");
	addSpriteSheetToSpriteMap(levelTilesSpriteSheet, "res/sprites/LevelTiles.json");

	Texture2D levelTileShadowRight = LoadTexture("res/sprites/LevelTileShadowRight.png");
	addSingleSpriteToSpriteMap(levelTileShadowRight, "TileShadowRight");

	Texture2D levelTileShadowBottem = LoadTexture("res/sprites/LevelTileShadowBottem.png");
	addSingleSpriteToSpriteMap(levelTileShadowBottem, "TileShadowBottem");

	textures.push_back(mainSpriteSheet);
	textures.push_back(levelTilesSpriteSheet);
	textures.push_back(levelTileShadowRight);
	textures.push_back(levelTileShadowBottem);
}

void SpriteManager::addSpriteSheetToSpriteMap(const Texture2D& spriteSheet, const std::string& sliceInformationFilepath) {
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
		sprites.emplace_back(spriteSheet, rect, xOffset, yOffset);
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

void SpriteManager::addSingleSpriteToSpriteMap(const Texture2D &sprite, const std::string& name) {
	Rectangle rect = { 0, 0, (float)sprite.width, (float)sprite.height };
	sprites.emplace_back(sprite, rect, 0, 0);
	spriteMap.insert({name, (SpriteHandle)(sprites.size() - 1)});
}
