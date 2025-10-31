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

COLLIDER_LAYER(Collider)

#define MULTI_COLLIDER_LAYER(name, count) \
struct name {\
	Collider colliders[count];\
	void flipX(int boxWidth) {\
		for (Collider& col : colliders) {\
			col.flipX(boxWidth);\
		}\
	}\
};

