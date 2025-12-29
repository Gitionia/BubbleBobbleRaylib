#pragma once

#include <raylib.h>
#include <raymath.h>

Rectangle ScaleRect(const Rectangle &rectangle, float scale);
Rectangle OffsetRect(const Rectangle &rectangle, float xOffset, float yOffset);

int sign(int x);

struct Vector2Int {
    int X, Y;

    static Vector2Int Zero() {
        return {0, 0};
    }
    static Vector2Int Up() {
        return {0, 1};
    }
    static Vector2Int Down() {
        return {0, -1};
    }
    static Vector2Int Right() {
        return {1, 0};
    }
    static Vector2Int Left() {
        return {-1, 0};
    }

    Vector2Int &operator+=(const Vector2Int &other) {
        X += other.X;
        Y += other.Y;
        return *this;
    }

    void IntegerNormalize() {
        X = sign(X);
        Y = sign(Y);
    }
};
