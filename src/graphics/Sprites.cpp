#include "Sprites.h"

#include "raylib.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "../utils/Log.h"
#include "nlohmann/json.hpp"

static std::vector<Texture2D> textures;
static std::vector<Sprite> sprites;
static std::unordered_map<std::string, SpriteHandle> spriteMap;


void addSpriteSheetToSpriteMap(const Texture2D& spriteSheet, const std::string& sliceInformationFilepath);
void addSingleSpriteToSpriteMap(const Texture2D& sprite, const std::string& name);

/**
 * Needs to be called before GetSprite(...) can be used.
 * Loads sprites from sprite sheets into spritemap.
 */
void LoadSprites()
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

void UnloadSprites() {
	for (auto& texture : textures) {
		UnloadTexture(texture);
	}
}

SpriteHandle GetSpriteHandle(const std::string &name) {
	return spriteMap.at(name);
}

SpriteHandle GetSpriteHandleChecked(const std::string &name) {
	if (spriteMap.find(name) == std::end(spriteMap)) {
		return -1;
	}
	return GetSpriteHandle(name);
}

const Sprite& GetSprite(SpriteHandle handle) {
	return sprites.at(handle);
}

void addSingleSpriteToSpriteMap(const Texture2D &sprite, const std::string& name) {
	Rectangle rect = { 0, 0, (float)sprite.width, (float)sprite.height };
	sprites.emplace_back(sprite, rect, 0, 0);
	spriteMap.insert({name, (SpriteHandle)(sprites.size() - 1)});
}

void addSpriteSheetToSpriteMap(const Texture2D& spriteSheet, const std::string& sliceInformationFilepath) {
	std::ifstream f(sliceInformationFilepath);
	if (f.fail()) {
		PRINT_ERROR("Spritesheet information at {} is missing!", sliceInformationFilepath);
		return;
	}
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
