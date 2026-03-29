#pragma once

#include "../app/Config.h"
#include "../graphics/Animations.h"
#include "../graphics/Sprites.h"
#include "../level/Enemies.h"
#include "../level/Items.h"
#include "../utils/AnimatedObjects.h"
#include "ComponentUtils.h"

struct Position {
    int x, y;

    int dir = -1;

    Vector2Int toVector() const {
        return {x, y};
    }

    Vector2 toRLVector() const {
        return {(float)x, (float)y};
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
    static inline const Collider bubbleRepelCollider{BP_SIZE(0, 14), BP_SIZE(1, 0), BP_SIZE(0, 7), BP_SIZE(0, 8)};
    // Bubble has width 28 Pixels and height 2 Blocks = 32 Pixel.
    // This is centered collider with half width and height
    static inline const Collider bubblePopCollider{BP_SIZE(2, 4), BP_SIZE(2, 8), BP_SIZE(0, -4), BP_SIZE(0, -4)};
    static inline const Collider bubbleJumpableCollider{BP_SIZE(0, 28), BP_SIZE(0, 4), 0, BP_SIZE(0, -2)};

    // Needs to be resized, because right now it works like a 2x2-block collider
    static inline const DragonSpikeCollider dragonSpikeCollider{BP_SIZE(0, 12), BP_SIZE(2, -4), BP_SIZE(2, -12), 0, // Spikes on the back
                                                                BP_SIZE(2, 0), BP_SIZE(0, 4), 0, BP_SIZE(2, -6)};

  private:
    constexpr static inline int l = 17;            // length of flying collider
    constexpr static inline int w = 2;             // width of flying collider
    constexpr static inline int o = BP_SIZE(0, 2); // width of flying collider

  public:
    static inline const Collider flyingTopLeftHorizontal{BP_SIZE(0, l), BP_SIZE(0, w), o, o};
    static inline const Collider flyingTopRightHorizontal{BP_SIZE(0, l), BP_SIZE(0, w), BP_SIZE(2, -l) - o, o};
    static inline const Collider flyingBottemLeftHorizontal{BP_SIZE(0, l), BP_SIZE(0, w), o, BP_SIZE(2, -w) - o};
    static inline const Collider flyingBottemRightHorizontal{BP_SIZE(0, l), BP_SIZE(0, w), BP_SIZE(2, -l) - o, BP_SIZE(2, -w) - o};

    static inline const Collider flyingTopLeftVertical{BP_SIZE(0, w), BP_SIZE(0, l), o, o};
    static inline const Collider flyingTopRightVertical{BP_SIZE(0, w), BP_SIZE(0, l), BP_SIZE(2, -w) - o, o};
    static inline const Collider flyingBottemLeftVertical{BP_SIZE(0, w), BP_SIZE(0, l), o, BP_SIZE(2, -l) - o};
    static inline const Collider flyingBottemRightVertical{BP_SIZE(0, w), BP_SIZE(0, l), BP_SIZE(2, -w) - o, BP_SIZE(2, -l) - o};
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
    Animator animator{&GetAnimation("Dragon-Idle")};

    int invincibilityFramesLeft = 0;
    static constexpr int INVINCIBILITY_FRAME_COUNT = 2.5f * 60;
    bool isInvincible() {
        return invincibilityFramesLeft > 0;
    }

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

    AnimationState state = IDLE;

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

struct EnemyInfoComponent {
    EnemyType type;
};

struct FlyingEnemyComponent {
    enum FlyingDirection {
        UP_RIGHT,
        DOWN_RIGHT,
        DOWN_LEFT,
        UP_LEFT
    };
    FlyingDirection dir;

    int getXDir() {
        switch (dir) {

        case UP_RIGHT:
            return 1;
        case DOWN_RIGHT:
            return 1;
        case DOWN_LEFT:
            return -1;
        case UP_LEFT:
            return -1;
        }
    }
    int getYDir() {
        switch (dir) {

        case UP_RIGHT:
            return -1;
        case DOWN_RIGHT:
            return 1;
        case DOWN_LEFT:
            return 1;
        case UP_LEFT:
            return -1;
        }
    }
    void flipX() {
        switch (dir) {

        case UP_RIGHT:
            dir = UP_LEFT;
            break;
        case DOWN_RIGHT:
            dir = DOWN_LEFT;
            break;
        case DOWN_LEFT:
            dir = DOWN_RIGHT;
            break;
        case UP_LEFT:
            dir = UP_RIGHT;
            break;
        }
    }
    void flipY() {
        switch (dir) {

        case UP_RIGHT:
            dir = DOWN_RIGHT;
            break;
        case DOWN_RIGHT:
            dir = UP_RIGHT;
            break;
        case DOWN_LEFT:
            dir = UP_LEFT;
            break;
        case UP_LEFT:
            dir = DOWN_LEFT;
            break;
        }
    }

    Collider getVerticalCollider() {
        switch (dir) {

        case UP_RIGHT:
            return Colliders::flyingTopRightVertical;
        case DOWN_RIGHT:
            return Colliders::flyingBottemRightVertical;
        case DOWN_LEFT:
            return Colliders::flyingBottemLeftVertical;
        case UP_LEFT:
            return Colliders::flyingTopLeftVertical;
        }
    }
    Collider getHorizontalCollider() {
        switch (dir) {

        case UP_RIGHT:
            return Colliders::flyingTopRightHorizontal;
        case DOWN_RIGHT:
            return Colliders::flyingBottemRightHorizontal;
        case DOWN_LEFT:
            return Colliders::flyingBottemLeftHorizontal;
        case UP_LEFT:
            return Colliders::flyingTopLeftHorizontal;
        }
    }
};

struct WalkingEnemyComponent {
    int walkingDir;
    // We only use a custom animator for the mushroom enemy to sync its animation with the jumps
    Animator animator {&GetAnimation(GetEnemyAnimationName(EnemyType::MUSHROOM, EnemyAnimationType::NORMAL))};
    bool isGapJumping = false;
};

struct EnemyTumbleComponent {
    Animator animator;

    AnimatedValue<int> xVel;
    AnimatedValue<int> yVel;
    ItemType itemToSpawn;
    bool ignoreCollision = true;
    bool isFalling = false;
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

    bool poppedFromLifeTime = true;
    // Says wether points should still be given for the pop.
    // Note that this can be set to true, even if no points were given,
    // because e. g. the bubble was an enemy where the pop gives no points
    bool gavePointsForPop = false;

    // The item level, depending on item levels of all bubbles that popped this bubble
    int itemLevel = 0;
};

struct ItemComponent {
    ItemType type;
};

struct PositionAnimationComponent {
    Vector2Int start;
    Vector2Int end;
    int totalFrameCount;
    int progress = 0;

    enum ActionWhenCompleted {
        DELETE_ENTITY
    };
    ActionWhenCompleted action;
};

struct ModifiableUITextComponent {
    std::string text;
    Color color;
    int fontSize = 32;
    int spacing = 0;
};

struct ConstUITextComponent {
    const char *text;
    Color color;
    int fontSize = 32;
    int spacing = 0;
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

struct DebugRectangle {
    int width;
    int height;
    Color color;
};

struct EnemyBubblePoppedTag {};
struct DestroyEntity {};

struct LevelTileTag {};
struct LevelTileShadowTag {};
struct DragonTag {};
struct BubbleTag {};
struct EnemyTag {};
struct EnemyTumbleTag {};
struct ItemTag {};
struct InGameTextTag {};
struct SimpleSpriteTag {};

// These Tags should never be removed from an entity since they identify the group of the entity

// An entity in gameplay (dragon, enemies, walls, bubbles, etc.) excluding any UI
struct GameplayEntityTag {};
struct UITextEntityTag {};
struct SpriteEntityTag {};

struct DebugDrawTag {};
