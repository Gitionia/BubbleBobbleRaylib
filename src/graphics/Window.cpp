#include "Window.h"

#include "raylib.h"

Window::Window(int width, int height, const char* title) :
	width(width), height(height), title(title)
{
	SetTraceLogLevel(LOG_ERROR);
	SetTargetFPS(60);
}

Window::~Window()
{
	CloseWindow();
}

bool Window::isOpen()
{
	return !WindowShouldClose();
}

void Window::init()
{
	InitWindow(width, height, title);
}
