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

struct RenderData {
	Sprite sprite;
};
