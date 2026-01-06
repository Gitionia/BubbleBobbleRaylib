#pragma once

struct Sprite {
    const Texture2D spriteSheet;
    const Rectangle coords;
    const int xOffset, yOffset;
    Sprite(const Texture2D spriteSheet, const Rectangle coords, int xOffset = 0, int yOffset = 0)
        : spriteSheet(spriteSheet), coords(coords), xOffset(xOffset), yOffset(yOffset) {}
};

typedef uint32_t SpriteHandle;

void LoadSprites();
void UnloadSprites();

SpriteHandle GetSpriteHandle(const std::string &name);
SpriteHandle GetSpriteHandleChecked(const std::string &name);
const Sprite &GetSprite(SpriteHandle handle);
