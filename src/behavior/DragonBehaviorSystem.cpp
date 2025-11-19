#include "DragonBehaviorSystem.h"

#include <iostream>

#include "../ecs/Components.h"
#include "Physics.h"
#include "../app/Config.h"
#include "../graphics/Animations.h"

constexpr int dragonJumpingSpeeds[] = {
	0, 2, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0
};

constexpr int jumpAnimationLength = sizeof(dragonJumpingSpeeds) / sizeof(int);

void DragonBehaviorSystem::Init() {
}

void DragonBehaviorSystem::Update() {
	SetGamepadMappings("0300000032150000290a000001010000,Razer Wolverine V2,a:b0,b:b1,x:b2,y:b3,back:b6,guide:b8,start:b7,leftstick:b9,rightstick:b10,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:a2,righttrigger:a5,crc:3361");

	static Animator animator(&GetAnimation("Dragon-Idle"));

	auto view = registry.view<Position, WalkingActorComponent, RenderData, Collider, DragonSpikeCollider>();
	for (auto entity : view) {
		auto [pos, actor, renderData, collider, dragonSpikes] = view.get(entity);


		if (animator.Update())
			animator.Reset();
		renderData.spriteHandle = animator.GetSpriteHandle();


		bool useGamepad = true;

		int velx = 0;
		int vely = 0;
		bool jump = (IsKeyDown(KEY_SPACE) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
		int moveSpeed = UNITS_PER_BLOCK / 16;
		int jumpSpeed = UNITS_PER_BLOCK / 8;
		int fallSpeed = UNITS_PER_BLOCK / 8;
		int JUMP_FRAME_COUNT = 5 * (int)(UNITS_PER_BLOCK * 1.1f) / jumpSpeed;

		int BOTTEM_WARP_POS = 27 * UNITS_PER_BLOCK;
		int TOP_WARP_POS = 30 * UNITS_PER_BLOCK;


		float deadZone = 0.4f;
		if (useGamepad && IsGamepadAvailable(0)) {
			float axisx = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
			// float axisy = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			// std::cout << "x:" << axisx << ", y: "<< axisy << std::endl;
			velx = (axisx > deadZone? 1 : axisx < -deadZone ?-1:0) * moveSpeed;
			// vely = axisy > deadZone? 1 : axisy < -deadZone ?-1:0;
		} else {
			if (IsKeyDown(KEY_RIGHT)) {
				velx += moveSpeed;
			}
			if (IsKeyDown(KEY_LEFT)) {
				velx -= moveSpeed;
			}
		}

        bool targetFilp;
		if (velx > 0) {
			targetFilp = true;
		} else if (velx < 0) {
			targetFilp = false;
		}

        if (velx != 0) {
            if (targetFilp != renderData.flipX) {
                dragonSpikes.flipX(2 * UNITS_PER_BLOCK);
            }
            renderData.flipX = targetFilp;
        }
        

		// check if should restore collision detection
		// if (actor.ignoreCollisions) {
		// 	if (!collidesWithWall(registry, pos, collider)) {
		// 		actor.ignoreCollisions = false;
		// 	}
		// }

        if (IsKeyDown(KEY_A) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
            factory.CreateBubble(pos, renderData.flipX ? 1 : -1);
        }

		// Above and below the level the dragon should ignore collisions.
		// Above includes all y-positions where the dragon would be standing on the
		// top of the level or above that.
		if (pos.y > 24 * UNITS_PER_BLOCK || pos.y <= -2 * UNITS_PER_BLOCK) {
			actor.ignoreCollisions = true;
		} else {
			actor.ignoreCollisions = collidesWithWall(registry, pos, collider);
		}

		// check if grounded
		bool isGrounded = false;
		if (!actor.isJumping) {

			pos.y += fallSpeed;
			if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
				isGrounded = true;
			}
			pos.y -= fallSpeed;
		}

		// start jump
        if (jump) {
            if (isGrounded || collidesWithCollider<BubbleJumpableTopCollider>(registry, pos, collider)) {
                actor.isJumping = true;
                actor.jumpFrameCount = 0;
            }
        }

		// execute jump
		if (actor.isJumping) {
			actor.jumpFrameCount++;
			// vely = -2 * dragonJumpingSpeeds[actor.jumpFrameCount];
			vely = -jumpSpeed;

			if (actor.jumpFrameCount == JUMP_FRAME_COUNT - 1) {
				actor.isJumping = false;
			}
		} else {
			vely = fallSpeed;
		}

		pos.x += velx;
		if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
			pos.x -= velx;
		}
		pos.x = std::max(2 * UNITS_PER_BLOCK, pos.x);
		pos.x = std::min(28 * UNITS_PER_BLOCK, pos.x);


		pos.y += vely;
		if (!actor.isJumping) {
			if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
				pos.y -= vely;
			}

			if (pos.y >= BOTTEM_WARP_POS) {
				pos.y -= TOP_WARP_POS;
			}
		}
	}

}

