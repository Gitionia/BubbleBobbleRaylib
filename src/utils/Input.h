#pragma once

#include "../level/Dragons.h"
#include "Recorder.h"
#include <memory>

enum class Key {
    None,
    Jump,
    Fire,
};

enum class InputConfiguration {
    SINGLEPLAYER,
    MULTIPLAYER
};

class Input {
  public:
    static void Init(std::shared_ptr<InputSimulator> simulator);

    static void UpdateTouchInput();

    static bool IsKeyDown(Key key, DragonColor playerColor);
    static bool IsKeyDown(int key);
    static bool IsKeyPressed(int key);
    static bool AnyKeyPressed();
    static int GetXAxis(DragonColor color);
    static int GetYAxis();

    static void SetInputConfiguration(InputConfiguration config);

  private:
    static inline bool useGamepad = true;
    static inline bool useTouchInput = true;
    static inline float deadZone = 0.4f;
};
