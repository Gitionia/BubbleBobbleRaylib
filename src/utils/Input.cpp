#include "Input.h"
#include "Recorder.h"
#include "Utilities.h"
#include "raylib.h"


static std::shared_ptr<InputSimulator> inputSimulator;
static InputConfiguration config;

#define MAX_TOUCH_POINTS 10
static Vector2 touchPositions[MAX_TOUCH_POINTS] {};
static int touchCount = 0;

void Input::Init(std::shared_ptr<InputSimulator> _inputSimulator) {
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

bool Input::IsKeyDown(Key key, DragonColor playerColor) {
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

    int jumpKey = KEY_SPACE;
    int fireKey = KEY_A;
    bool useGamepadInput = false;

    if (config == InputConfiguration::SINGLEPLAYER) {
        jumpKey = KEY_SPACE;
        fireKey = KEY_A;
        useGamepadInput = true;

    } else if (config == InputConfiguration::MULTIPLAYER) {

        if (playerColor == DRAGON_GREEN) {
            jumpKey = KEY_UP;
            fireKey = KEY_L;
            useGamepadInput = false;

        } else {
            jumpKey = KEY_S;
            fireKey = KEY_T;
            useGamepadInput = true;
        }
    }

    switch (key) {
    case Key::Jump:
        return inputSimulator->IsKeyDown(jumpKey) || (useGamepadInput && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
    case Key::Fire:
        return inputSimulator->IsKeyDown(fireKey) || (useGamepadInput && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT));
    default:
        return false;
    }
}

bool Input::AnyKeyPressed() {
    return GetKeyPressed() != 0 || GetGamepadButtonPressed() != 0 || GetTouchPointCount() > 0;
}

int Input::IsNumberKeyDown() {
    if (inputSimulator->IsKeyDown(KEY_ONE)) {
        return 1;
    } else if (inputSimulator->IsKeyDown(KEY_TWO)) {
        return 2;
    } else {
        return -1;
    }
}


int Input::GetXAxis(DragonColor color) {

    int dir = 0;

    if (config == InputConfiguration::SINGLEPLAYER || color == DRAGON_BLUE) {
        if (useGamepad) {
            float axis = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            dir += axis > deadZone ? 1 : axis < -deadZone ? -1
                                                          : 0;
        }
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

    int keyRight = KEY_RIGHT;
    int keyLeft = KEY_LEFT;

    if (color == DRAGON_GREEN) {
        keyRight = KEY_RIGHT;
        keyLeft = KEY_LEFT;
    
    } else {
        keyRight = KEY_D;
        keyLeft = KEY_A;
    }

    if (inputSimulator->IsKeyDown(keyRight)) {
        dir += 1;
    }
    if (inputSimulator->IsKeyDown(keyLeft)) {
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

void Input::SetInputConfiguration(InputConfiguration _config) {
    config = _config;
}
