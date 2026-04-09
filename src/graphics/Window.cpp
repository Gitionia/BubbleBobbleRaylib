#include "Window.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

EM_BOOL emscripten_window_resized_callback(int eventType, const void *reserved, void *userData){
	return true;
}
#endif

Window::Window(int width, int height, const char *title) : width(width), height(height), title(title) {
    SetTraceLogLevel(LOG_ERROR);
    SetTargetFPS(60);

    // Starts web build in soft fullscreen
#ifdef PLATFORM_WEB
    EmscriptenFullscreenStrategy strategy;
    strategy.scaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
    strategy.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
    strategy.canvasResizedCallback = emscripten_window_resized_callback;
    strategy.canvasResizedCallbackUserData = nullptr;
    emscripten_enter_soft_fullscreen("canvas", &strategy);
#endif
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
