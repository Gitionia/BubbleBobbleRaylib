#pragma once

#include "../graphics/Sprites.h"
#include "raylib.h"
#include "../app/Config.h"
#include "ComponentUtils.h"

struct Position {
	int x, y;
};

struct Velocity {
	int dx, dy;
};

struct BallSize {
	int radius;
};

/*Could theoretically be a static instance somewhere, 
since it can be created and calculated*/
COLLIDER_LAYER(DragonSpikeCollider)
/* Could also be a static instance, just like DragonSpikeCollider*/
COLLIDER_LAYER(BubbleJumpableTopCollider)
COLLIDER_LAYER(Collider)


struct WalkingActorComponent {
	bool isJumping = false;
	bool ignoreCollisions = false;
	// number of frames left for the jump
	int jumpFrameCount = 0;
};

enum class BubbleState {
    SHOOTING,
    FLOATING
};

struct BubbleComponent {
    int shootDirection;
    BubbleState state = BubbleState::SHOOTING;
    int shootCounter = MAX_SHOOT_COUNTER;
	int lifetimeCounter = MAX_LIFE_TIME;
	static constexpr int MAX_LIFE_TIME = TARGET_FPS * 20;
	static constexpr int MAX_SHOOT_COUNTER = 80;
};


struct RenderData {
	SpriteHandle spriteHandle;
	Vector2 scale;
    int xoffset = 0;
    int yoffset = 0;
	bool flipX = false;
	bool flipY = false;

	RenderData(const SpriteHandle handle, const Vector2& scale, int xoffset, int yoffset)
		: spriteHandle(handle), scale(scale), xoffset(xoffset), yoffset(yoffset) {}
	RenderData(const SpriteHandle handle, const Vector2& scale)
		: spriteHandle(handle), scale(scale){}
	RenderData(const SpriteHandle handle, float scale)
		: spriteHandle(handle), scale({scale, scale}) {}
	explicit RenderData(const SpriteHandle handle)
		: spriteHandle(handle), scale({1, 1}) {}
};

struct DestroyEntity {};

struct LevelTileTag {};
struct DragonTag {};
struct BubbleTag {};
