#pragma once

#include "../app/Config.h"
#include "../graphics/Animations.h"
#include "../graphics/Sprites.h"
#include "ComponentUtils.h"
#include "raylib.h"

struct Position {
    int x, y;

    Vector2Int toVector() const {
        return {x, y};
    }
};

/*Could theoretically be a static instance somewhere,
since it can be created and calculated*/
MULTI_COLLIDER_LAYER(DragonSpikeCollider, 2)
// COLLIDER_LAYER(DragonSpikeCollider)
/* Could also be a static instance, just like DragonSpikeCollider*/
COLLIDER_LAYER(BubbleJumpableTopCollider)

struct WalkingActorComponent {
    bool isJumping = false;
    bool ignoreCollisions = false;
    // number of frames left for the jump
    int jumpFrameCount = 0;
};

struct DragonComponent {
    int bubbleShootDelay = 0;
    static constexpr int MAX_BUBBLE_SHOOT_DELAY = 1;
};

enum class BubbleState {
    SHOOTING,
    FLOATING
};

struct BubbleComponent {
    int shootDirection;
    Animator animator;

    BubbleState state = BubbleState::SHOOTING;
    int shootCounter = MAX_SHOOT_COUNTER;
    int lifetimeCounter = MAX_LIFE_TIME;
    // Value > 0, if is floating and the jumpable collider is not added yet
    int jumpableDelay = 0;
    // Value > 0, if is floating and it can't be popped yet
    // Should be less than jumpableDelay
    int popableDelay = 0;

    static constexpr int MAX_JUMPABLE_DELAY = 9;
    static constexpr int MAX_POPABLE_DELAY = 7;
    static constexpr int MAX_LIFE_TIME = TARGET_FPS * 20;
    static constexpr int MAX_SHOOT_COUNTER = 60;
};

struct RenderData {
    SpriteHandle spriteHandle;
    Vector2 scale;
    Color color = WHITE;
    int xoffset = 0;
    int yoffset = 0;
    bool flipX = false;
    bool flipY = false;

    RenderData(const SpriteHandle handle, const Vector2 &scale, int xoffset, int yoffset)
        : spriteHandle(handle), scale(scale), xoffset(xoffset), yoffset(yoffset) {}
    RenderData(const SpriteHandle handle, const Vector2 &scale)
        : spriteHandle(handle), scale(scale) {}
    RenderData(const SpriteHandle handle, float scale)
        : spriteHandle(handle), scale({scale, scale}) {}
    explicit RenderData(const SpriteHandle handle)
        : spriteHandle(handle), scale({1, 1}) {}

    RenderData &SetDirection(int dir) {
        if (dir > 0)
            flipX = true;
        else if (dir < 0)
            flipX = false;
        return *this;
    }

    RenderData &SetColor(Color color) {
        this->color = color;
        return *this;
    }
};

struct DebugCircle {
    float radius;
    Color color;
};

struct DestroyEntity {};

struct LevelTileTag {};
struct LevelTileShadowTag {};
struct DragonTag {};
struct BubbleTag {};

struct DebugDrawTag {};
