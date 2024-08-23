#pragma once

#include <entt/entt.hpp>


#include "../physics/PhysicsSystem.h"
#include "../graphics/RendererSystem.h"

class SystemRunner
{
public:
	explicit SystemRunner(entt::registry& registry);

	void UpdateSystems() const;
	void InitSystems();

private:
	PhysicsSystem physicsSystem;
	RendererSystem renderSystem;
};
