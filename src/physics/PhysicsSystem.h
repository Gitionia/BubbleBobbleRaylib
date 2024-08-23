#pragma once

#include "entt/entt.hpp"

class PhysicsSystem
{
public:
	explicit PhysicsSystem(entt::registry& registry);

	void update() const;

private:
	entt::registry& registry;
};