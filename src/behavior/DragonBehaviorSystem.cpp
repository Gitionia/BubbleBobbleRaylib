#include "DragonBehaviorSystem.h"

#include <iostream>

#include "../ecs/Components.h"
#include "Physics.h"

DragonBehaviorSystem::DragonBehaviorSystem(entt::registry& registry)
	: registry(registry)
{
}

void DragonBehaviorSystem::Update() const {
	SetGamepadMappings("0300000032150000290a000001010000,Razer Wolverine V2,a:b0,b:b1,x:b2,y:b3,back:b6,guide:b8,start:b7,leftstick:b9,rightstick:b10,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:a2,righttrigger:a5,crc:3361");

	auto view = registry.view<Position, WalkingActorComponent, RenderData, Collider>();
	for (auto entity : view) {
		auto [pos, actor, renderData, collider] = view.get(entity);

		bool useGamepad = true;

		int velx = 0;
		int vely = 0;
		bool jump = (IsKeyDown(KEY_SPACE) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
		int moveSpeed = 1;
		int jumpSpeed = 2;
		int fallSpeed = 2;
		int JUMP_FRAME_COUNT = 5 * 18 / jumpSpeed;


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
		if (velx > 0) {
			renderData.sprite.flipX = true;
		} else if (velx < 0) {
			renderData.sprite.flipX = false;
		}

		// check if should restore collision detection
		// if (actor.ignoreCollisions) {
		// 	if (!collidesWithWall(registry, pos, collider)) {
		// 		actor.ignoreCollisions = false;
		// 	}
		// }
		actor.ignoreCollisions = collidesWithWall(registry, pos, collider);

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
		if (isGrounded && jump) {
			actor.isJumping = true;
			actor.jumpFrameCount = JUMP_FRAME_COUNT;
		}

		// execute jump
		if (actor.isJumping) {
			actor.jumpFrameCount--;
			vely = -jumpSpeed;

			if (actor.jumpFrameCount == 0) {
				actor.isJumping = false;
			}
		} else {
			vely = fallSpeed;
		}

		pos.x += velx;
		if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
			pos.x -= velx;
		}

		pos.y += vely;
		if (!actor.isJumping) {
			if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
				pos.y -= vely;
			}
		}
	}

}
