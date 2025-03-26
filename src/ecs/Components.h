#pragma once

#include "../graphics/Sprites.h"
#include "raylib.h"

struct Position {
	int x, y;
};

struct Velocity {
	int dx, dy;
};

struct BallSize {
	int radius;
};

// Collider relative to a Position component
struct Collider {
	int width, height;
	// offset from top left corner
	int offsetX, offsetY;
};

struct DragonComponent {
	int state;
};


struct RenderData {
	Sprite sprite;
	Vector2 scale;

	RenderData(const Sprite& sprite, const Vector2& scale)
		: sprite(sprite), scale(scale) {}
	explicit RenderData(const Sprite& sprite)
		: sprite(sprite), scale({1, 1}) {}
};


struct LevelTileTag {};
struct DragonTag {};
