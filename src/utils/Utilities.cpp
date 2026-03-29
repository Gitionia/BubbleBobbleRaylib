#include "Utilities.h"
#include "Debug.h"


int GetIntFromDirection(Direction direction) {
    if (direction == Direction::Left) {
        return -1;
    } else {
        return 1;
    }
}
Direction GetDirectionFromInt(int dir) {
    DBG_CHECK(dir == 1 || dir == -1, "Tried to convert integer direction that is not 1 or -1, to Direction enum");
    if (dir > 0) {
        return Direction::Right;
    } else {
        return Direction::Left;
    }
}

Rectangle ScaleRect(const Rectangle &rectangle, float scale) {
    return {rectangle.x * scale, rectangle.y * scale, rectangle.width * scale, rectangle.height * scale};
}

Rectangle OffsetRect(const Rectangle &rectangle, float xOffset, float yOffset) {
    return {rectangle.x + xOffset, rectangle.y + yOffset, rectangle.width, rectangle.height};
}

int sign(int x) {
    if (x > 0)
        return 1;
    else if (x < 0)
        return -1;
    else
        return 0;
}
