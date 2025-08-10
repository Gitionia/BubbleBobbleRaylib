#pragma once

#define COLLIDER_LAYER(name) \
struct name {\
	int width, height;\
	int offsetX, offsetY;\
};