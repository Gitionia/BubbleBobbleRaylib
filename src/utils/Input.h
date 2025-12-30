#pragma once

enum class Key {
    None,
    Jump
};

class Input {
  public:
    static void Init();
      static bool IsKeyDown(Key key);
    static int GetXAxis();

  private:
    static inline bool useGamepad = true;
    static inline float deadZone = 0.4f;
};
