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

	Sprite& GetSprite(const std::string& name);

private:
	Texture2D mainSpriteSheet;
	std::unordered_map<std::string, Sprite> spriteMap;
};
