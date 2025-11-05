#pragma once

#include <array>
#include <cstdint>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Sprites.h"

struct Sprite {
	const Texture2D* spriteSheet;
	const Rectangle coords;
	const int xOffset, yOffset;
	Sprite(const Texture2D* spriteSheet, const Rectangle coords, int xOffset = 0, int yOffset = 0)
		: spriteSheet(spriteSheet), coords(coords), xOffset(xOffset), yOffset(yOffset) {}
};


typedef uint32_t SpriteHandle;
class SpriteManager {
public:
	SpriteManager();
	~SpriteManager();

	void LoadSprites();

	SpriteHandle GetSpriteHandle(const std::string &name) const;
	const Sprite& GetSprite(SpriteHandle handle) const;

private:
	void addSpritesToSpriteMap(const Texture2D& spriteSheet, const std::string& sliceInformationFilepath);
private:
	Texture2D mainSpriteSheet;
	Texture2D levelTilesSpriteSheet;
	std::vector<Sprite> sprites;
	std::unordered_map<std::string, SpriteHandle> spriteMap;
};
