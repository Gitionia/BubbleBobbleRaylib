#pragma once

#include "../app/Config.h"
#include "../graphics/Animations.h"
#include "../graphics/Sprites.h"
#include "../utils/AnimatedObjects.h"
#include "ComponentUtils.h"
#include "raylib.h"

struct Position {
    int x, y;

    int dir = -1;

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

class Colliders {
  public:
    static inline const Collider walkingActorCollider = {BP_SIZE(2, 0), BP_SIZE(1, 0), 0, BP_SIZE(1, 0)};
    static inline const Collider bubbleCollider{BP_SIZE(0, 28), BP_SIZE(2, 0), 0, 0};
    static inline const Collider bubbleJumpableCollider{BP_SIZE(0, 28), BP_SIZE(0, 4), 0, BP_SIZE(0, -2)};
    
    // Needs to be resized, because right now it works like a 2x2-block collider
    static inline const DragonSpikeCollider dragonSpikeCollider{BP_SIZE(0, 12), BP_SIZE(2, -4), BP_SIZE(2, -12), 0, // Spikes on the back
                                                                BP_SIZE(2, 0), BP_SIZE(0, 4), 0, BP_SIZE(2, -6)};
};

struct WalkingActorComponent {
    int fallSpeed;
    int jumpSpeed;
    bool ignoreCollisions = false;
    // number of frames left for the jump
    int jumpFrameCount = 0;

    bool isJumping() {
        return jumpFrameCount > 0;
    }
};

struct DragonComponent {
    int bubbleShootDelay = 0;

    static inline AnimatedValueDefinition<int> jumpSpeedsDefintion{{BP_SIZE(0, 6), 5}, {BP_SIZE(0, 4), 10}, {BP_SIZE(0, 2), 10}};
    AnimatedValue<int> jumpSpeed{jumpSpeedsDefintion};

    static constexpr int MAX_BUBBLE_SHOOT_DELAY = 30;
};

struct EnemyComponent {
    int walkingDir;
};

struct BubbleShootComponent {
    int shootDirection;
    Animator animator{&GetAnimation("Bubble-Green-Idle")};

    static constexpr int SHOOT_FRAME_COUNT = 60;
    int shootFrame = SHOOT_FRAME_COUNT;

    static constexpr int POPABLE_DELAY_FRAME_COUNT = 0;
    int popableDelayFrame = 0;

    static constexpr int JUMPABLE_DELAY_FRAME_COUNT = 3;
    int jumpableDelayFrame = 0;

    bool isWaiting() {
        return jumpableDelayFrame > 0;
    }
};

struct BubbleFloatComponent {
    Animator animator{&GetAnimation("Bubble-Green-Idle")};

    static constexpr int LIFETIME_FRAME_COUNT = 20 * TARGET_FPS;
    int lifetimeFrame = LIFETIME_FRAME_COUNT;
};

struct RenderData {
    SpriteHandle spriteHandle;
    Vector2 scale;
    Color color = WHITE;
    int xoffset = 0;
    int yoffset = 0;

    RenderData(const SpriteHandle handle, const Vector2 &scale, int xoffset, int yoffset)
        : spriteHandle(handle), scale(scale), xoffset(xoffset), yoffset(yoffset) {}
    RenderData(const SpriteHandle handle, const Vector2 &scale)
        : spriteHandle(handle), scale(scale) {}
    RenderData(const SpriteHandle handle, float scale)
        : spriteHandle(handle), scale({scale, scale}) {}
    explicit RenderData(const SpriteHandle handle)
        : spriteHandle(handle), scale({1, 1}) {}

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
struct EnemyTag {};

struct DebugDrawTag {};
