#pragma once

#include "raylib.h"

class Window {
public:
	Window(int width, int height, const char* title);
	~Window();

	bool isOpen();
	void init();

private:
	int width;
	int height;
	const char* title;
};