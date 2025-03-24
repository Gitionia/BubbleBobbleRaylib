#include "PhysicsSystem.h"

#include "../ecs/Components.h"
#include "raylib.h"

PhysicsSystem::PhysicsSystem(entt::registry& registry)
	: registry(registry)
{
}

void PhysicsSystem::Update() const
{
	auto view = registry.view<Position, Velocity, BallSize>();
	for (auto entity : view) {
		auto [pos, vel, size] = view.get(entity);

		pos.x += vel.dx;
		pos.y += vel.dy;

		// Check walls collision for bouncing
		if ((pos.x >= (GetScreenWidth() - size.radius)) || (pos.x <= size.radius)) vel.dx *= -1.0f;
		if ((pos.y >= (GetScreenHeight() - size.radius)) || (pos.y <= size.radius)) vel.dy *= -1.0f;

	}
}
