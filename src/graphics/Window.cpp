#include "Window.h"

Window::Window(int width, int height, const char *title) : width(width), height(height), title(title) {
    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(60);
}

Window::~Window() {
    CloseWindow();
}

bool Window::IsOpen() {
    return !WindowShouldClose();
}

void Window::Init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, title);

    // ToggleFullscreen();
}

void Window::SetFPS(int fps) {
    SetTargetFPS(fps);
}
