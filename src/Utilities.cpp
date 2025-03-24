#include "Utilities.h"

Rectangle ScaleRect(const Rectangle& rectangle, float scale) {
	return {rectangle.x * scale, rectangle.y * scale, rectangle.width * scale, rectangle.height * scale };
}