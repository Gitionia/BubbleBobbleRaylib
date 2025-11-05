#pragma once

#include <raylib.h>
#include <raymath.h>

Rectangle ScaleRect(const Rectangle& rectangle, float scale);
Rectangle OffsetRect(const Rectangle& rectangle, float xOffset, float yOffset);
