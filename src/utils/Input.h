#pragma once

enum class Key {
    None,
    Jump,
    Fire,
    Any
};

class Input {
  public:
    static void Init();
    static bool IsKeyDown(Key key);
    static int GetXAxis();
    static int GetYAxis();

  private:
    static inline bool useGamepad = true;
    static inline float deadZone = 0.4f;
};
