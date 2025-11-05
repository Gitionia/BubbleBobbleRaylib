#include "Utilities.h"

Rectangle ScaleRect(const Rectangle& rectangle, float scale) {
	return {rectangle.x * scale, rectangle.y * scale, rectangle.width * scale, rectangle.height * scale };
}

Rectangle OffsetRect(const Rectangle& rectangle, float xOffset, float yOffset) {
	return {rectangle.x + xOffset, rectangle.y + yOffset, rectangle.width, rectangle.height};
}

