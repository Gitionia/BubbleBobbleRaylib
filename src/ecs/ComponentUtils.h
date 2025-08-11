#pragma once

#define COLLIDER_LAYER(name) \
struct name {\
	int width, height;\
	int offsetX, offsetY;\
    /*Flips this collider on the x-Axis of an entity with width 'boxwidth'*/\
    void flipX(int boxWidth) {\
        offsetX = boxWidth - (offsetX + width);\
    }\
};