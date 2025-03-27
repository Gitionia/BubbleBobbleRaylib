#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

struct Sprite {
	Texture2D& spriteSheet;
	Rectangle coords;
};

class SpriteManager {
public:
	SpriteManager();
	~SpriteManager();

	void LoadSprites();

	Sprite GetSprite(const std::string &name) const;

private:
	void addSpritesToSpriteMap(Texture2D& spriteSheet, const std::string& sliceInformationFilepath);

private:
	Texture2D mainSpriteSheet;
	Texture2D levelTilesSpriteSheet;
	std::unordered_map<std::string, Sprite> spriteMap;
};
