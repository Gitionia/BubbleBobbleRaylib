#include "DragonBehaviorSystem.h"

#include <iostream>

#include "../ecs/Components.h"

DragonBehaviorSystem::DragonBehaviorSystem(entt::registry& registry)
	: registry(registry)
{
}

void DragonBehaviorSystem::Update() const {
	SetGamepadMappings("0300000032150000290a000001010000,Razer Wolverine V2,a:b0,b:b1,x:b2,y:b3,back:b6,guide:b8,start:b7,leftstick:b9,rightstick:b10,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:a2,righttrigger:a5,crc:3361");

	auto view = registry.view<Position, DragonComponent, RenderData>();
	for (auto entity : view) {
		auto [pos, dragon, renderData] = view.get(entity);

		int speed = 2;
		bool useGamepad = true;

		int velx = 0;
		int vely = 0;

		float deadZone = 0.4f;
		if (useGamepad && IsGamepadAvailable(0)) {
			float axisx = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
			float axisy = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
			std::cout << "x:" << axisx << ", y: "<< axisy << std::endl;
			velx = axisx > deadZone? 1 : axisx < -deadZone ?-1:0;
			vely = axisy > deadZone? 1 : axisy < -deadZone ?-1:0;
		} else {
			if (IsKeyDown(KEY_RIGHT)) {
				velx += speed;
				renderData.sprite.flipX = true;
			}
			if (IsKeyDown(KEY_LEFT)) {
				velx -= speed;
				renderData.sprite.flipX = false;
			}
			if (IsKeyDown(KEY_UP)) {
				vely -= speed;
			}
			if (IsKeyDown(KEY_DOWN)) {
				vely += speed;
			}
		}

		pos.x += velx;
		pos.y += vely;
	}

}
