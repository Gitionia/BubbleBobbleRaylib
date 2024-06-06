#pragma once

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

struct Sprite {
	Rectangle coords;
};
