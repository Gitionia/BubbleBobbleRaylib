#pragma once

class Window {
  public:
    Window(int width, int height, const char *title);
    ~Window();

    bool IsOpen();
    void Init();
    void SetFPS(int fps);

  private:
    int width;
    int height;
    const char *title;
};
