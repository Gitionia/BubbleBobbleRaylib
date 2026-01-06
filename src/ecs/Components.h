#pragma once

#include "../app/Config.h"
#include "../graphics/Animations.h"
#include "../graphics/Sprites.h"
#include "../utils/AnimatedObjects.h"
#include "ComponentUtils.h"

struct Position {
    int x, y;

    int dir = -1;

    Vector2Int toVector() const {
        return {x, y};
    }

    void set(const Position &pos) {
        set(pos.x, pos.y, pos.dir);
    }

    void set(int x, int y, int dir) {
        this->x = x;
        this->y = y;
        this->dir = dir;
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
    static inline const Collider fullActorCollider = {BP_SIZE(2, 0), BP_SIZE(2, 0), 0, 0};
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

    void reset(int fallSpeed, int jumpSpeed) {
        jumpFrameCount = 0;
        ignoreCollisions = false;
        this->fallSpeed = fallSpeed;
        this->jumpSpeed = jumpSpeed;
    }
};

struct DragonComponent {
    Animator animator{&GetAnimation("Dragon-Idle")};

    int bubbleShootDelay = 0;

    static inline AnimatedValueDefinition<int> jumpSpeedsDefintion{{BP_SIZE(0, 6), 5}, {BP_SIZE(0, 4), 10}, {BP_SIZE(0, 2), 10}};
    AnimatedValue<int> jumpSpeed{jumpSpeedsDefintion};

    static constexpr int MAX_BUBBLE_SHOOT_DELAY = 30;

    enum AnimationState {
        IDLE,
        WALKING,
        SHOOTING,
        JUMPING,
        FALLING
    };

    AnimationState state;

    void reset() {
        animator.SetNewAnimation(&GetAnimation("Dragon-Idle"));
        bubbleShootDelay = 0;
        jumpSpeed.reset();
        state = IDLE;
    }

    static constexpr int FALL_SPEED = UNITS_PER_BLOCK / 8;
    static constexpr int JUMP_SPEED = 3 * UNITS_PER_BLOCK / 16;

    static inline const Position STARTING_POSITION{BP_SIZE(3, 0), BP_SIZE(23, 0), 1};
};

struct DragonHitComponent {
    Animator animator{&GetAnimation("Dragon-Hit")};

    enum AnimationState {
        HIT = 0,
        HIT_STARE = 1,
        RESPAWN = 2
    };

    AnimationState state = HIT;

    int repetitionCount = 4;
};

struct EnemyComponent {
    int walkingDir;
};

struct BubbleShootComponent {
    int shootDirection;
    Animator animator{&GetAnimation("Bubble-Green-Shoot")};

    static constexpr int SHOOT_FRAME_COUNT = 24;
    int shootFrame = SHOOT_FRAME_COUNT;

    int popableDelayFrame = 0;

    int jumpableDelayFrame = 0;

    bool isWaiting() {
        return jumpableDelayFrame > 0;
    }

    enum State {
        // If no state was determined yet
        NONE,
        // No wall collision when shot first
        NORMAL_SHOOT,
        // Wall collision when shot, but can shoot outside the wall
        IGNORE_COLLISION_SHOOT,
        // Wall collision when shot and can't shoot outside the wall
        IGNORE_COLLISION_WAIT
    };

    State state = NONE;
    bool ignoreCollision = false;
    int ignoreCollisionWaitFrame = 0;
};

struct BubbleFloatComponent {
    Animator animator{&GetAnimation("Bubble-Green-Idle")};

    static constexpr int LIFETIME_FRAME_COUNT = 20 * TARGET_FPS;
    int lifetimeFrame = LIFETIME_FRAME_COUNT;

    // Value > 0, if bubble was shot in ignore collision wait and should be some frames jumpable
    int popFrame = 0;
    // If this is true, then bubble should not be popable by dragon spikes and only pop when popFrame reaches 0
    bool isWaitingForPop() {
        return popFrame > 0;
    }
};

struct BubblePopComponent {
    Animator animator{&GetAnimation("Bubble-PrePop")};
    bool isInStatePrePop = true;
    int popAnimationRepetitions = 0;
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
