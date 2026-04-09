#include "Input.h"
#include "Recorder.h"
#include "Utilities.h"
#include "raylib.h"

#include "../app/WindowConfig.h"

static std::shared_ptr<InputSimulator> inputSimulator;

#define MAX_TOUCH_POINTS 10
static Vector2 touchPositions[MAX_TOUCH_POINTS] = {0};
static int touchCount = 0;

void Input::Init(std::shared_ptr<InputSimulator> _inputSimulator) {
    // SetGamepadMappings("0300000032150000290a000001010000,Razer Wolverine V2,a:b0,b:b1,x:b2,y:b3,back:b6,guide:b8,start:b7,leftstick:b9,rightstick:b10,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:a2,righttrigger:a5,crc:3361");
    ::inputSimulator = _inputSimulator;
}

void Input::UpdateTouchInput() {
    if (useTouchInput) {
        touchCount = GetTouchPointCount();

        if (touchCount > MAX_TOUCH_POINTS) {
            touchCount = MAX_TOUCH_POINTS;
        }

        for (int i = 0; i < touchCount; i++) {
            touchPositions[i] = GetTouchPosition(i);
        }
    }
}

bool Input::IsKeyDown(int key) {
    return inputSimulator->IsKeyDown(key);
}

bool Input::IsKeyPressed(int key) {
    return inputSimulator->IsKeyPressed(key);
}

bool Input::IsKeyDown(Key key) {
    if (useTouchInput) {
        for (int i = 0; i < touchCount; i++) {
            Vector2 touchPos = touchPositions[i];

            if (touchPos.x >= GetScreenWidth() / 2.0f) {
                if (key == Key::Jump && touchPos.y >= GetScreenHeight() / 2.0f) {
                    return true;
                } else if (key == Key::Fire && touchPos.y < GetScreenHeight() / 2.0f) {
                    return true;
                }
            }
        }
    }

    switch (key) {
    case Key::Jump:
        return inputSimulator->IsKeyDown(KEY_SPACE) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    case Key::Fire:
        return inputSimulator->IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
    case Key::Any:
        return GetKeyPressed() != 0 || GetGamepadButtonPressed() != 0 || GetTouchPointCount() > 0;
    default:
        return false;
    }
}

int Input::GetXAxis() {

    int dir = 0;

    if (useGamepad) {
        float axis = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        dir += axis > deadZone ? 1 : axis < -deadZone ? -1
                                                      : 0;
    }

    if (useTouchInput) {
        int touchCount = GetTouchPointCount();

        for (int i = 0; i < touchCount; i++) {
            Vector2 touchPos = touchPositions[i];
            if (touchPos.x < GetScreenWidth() / 4.0f) {
                dir += -1;
                break;
            } else if (touchPos.x < GetScreenWidth() / 2.0f) {
                dir += 1;
                break;
            }
        }
    }

    if (inputSimulator->IsKeyDown(KEY_RIGHT)) {
        dir += 1;
    }
    if (inputSimulator->IsKeyDown(KEY_LEFT)) {
        dir += -1;
    }

    dir = sign(dir);

    return dir;
}

int Input::GetYAxis() {

    if (useGamepad && IsGamepadAvailable(0)) {
        float axis = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
        return axis > deadZone ? 1 : axis < -deadZone ? -1
                                                      : 0;

    } else {
        int dir = 0;

        if (inputSimulator->IsKeyDown(KEY_UP)) {
            dir += 1;
        }
        if (inputSimulator->IsKeyDown(KEY_DOWN)) {
            dir += -1;
        }

        return dir;
    }
}
