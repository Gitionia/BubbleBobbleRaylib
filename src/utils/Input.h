#pragma once

#include "Recorder.h"
#include <memory>

enum class Key {
    None,
    Jump,
    Fire,
    Any
};

class Input {
  public:
    static void Init(std::shared_ptr<InputSimulator> simulator);
    
    static void UpdateTouchInput();
    
    static bool IsKeyDown(Key key);
    static bool IsKeyDown(int key);
    static bool IsKeyPressed(int key);
    static int GetXAxis();
    static int GetYAxis();

  private:
    // static std::shared_ptr<InputSimulator> inputSimulator;
    static inline bool useGamepad = true;
    static inline bool useTouchInput = true;
    static inline float deadZone = 0.4f;
};
