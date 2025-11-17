#pragma once

#include <array>
#include <cstdint>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Sprites.h"

struct Sprite {
	const Texture2D spriteSheet;
	const Rectangle coords;
	const int xOffset, yOffset;
	Sprite(const Texture2D spriteSheet, const Rectangle coords, int xOffset = 0, int yOffset = 0)
		: spriteSheet(spriteSheet), coords(coords), xOffset(xOffset), yOffset(yOffset) {}
};


typedef uint32_t SpriteHandle;
class SpriteManager {
public:
	SpriteManager() = default;
	~SpriteManager();

	void LoadSprites();

	SpriteHandle GetSpriteHandle(const std::string &name) const;
	SpriteHandle GetSpriteHandleChecked(const std::string& name) const;
	const Sprite& GetSprite(SpriteHandle handle) const;

private:
	void addSingleSpriteToSpriteMap(const Texture2D& sprite, const std::string& name);
	void addSpriteSheetToSpriteMap(const Texture2D& spriteSheet, const std::string& sliceInformationFilepath);
private:
	std::vector<Texture2D> textures;
	std::vector<Sprite> sprites;
	std::unordered_map<std::string, SpriteHandle> spriteMap;
};
