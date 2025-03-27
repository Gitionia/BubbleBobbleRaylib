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

struct WalkingActorComponent {
	bool isJumping = false;
	bool ignoreCollisions = false;
	// number of frames left for the jump
	int jumpFrameCount = 0;
};


struct RenderData {
	SpriteHandle spriteHandle;
	Vector2 scale;
	bool flipX = false;
	bool flipY = false;

	RenderData(const SpriteHandle handle, const Vector2& scale)
		: spriteHandle(handle), scale(scale) {}
	explicit RenderData(const SpriteHandle handle)
		: spriteHandle(handle), scale({1, 1}) {}
};


struct LevelTileTag {};
struct DragonTag {};
