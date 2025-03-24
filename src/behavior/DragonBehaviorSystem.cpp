#include "DragonBehaviorSystem.h"

#include "../ecs/Components.h"

DragonBehaviorSystem::DragonBehaviorSystem(entt::registry& registry)
	: registry(registry)
{
}

void DragonBehaviorSystem::Update() const {
	auto view = registry.view<Position, DragonComponent>();
	for (auto entity : view) {
		auto [pos, dragon] = view.get(entity);

		int speed = 1;

		int velx = 0;
		int vely = 0;
		if (IsKeyDown(KEY_RIGHT)) {
			velx += speed;
		}
		if (IsKeyDown(KEY_LEFT)) {
			velx -= speed;
		}
		if (IsKeyDown(KEY_UP)) {
			vely -= speed;
		}
		if (IsKeyDown(KEY_DOWN)) {
			vely += speed;
		}

		pos.x += velx;
		pos.y += vely;
	}

}
