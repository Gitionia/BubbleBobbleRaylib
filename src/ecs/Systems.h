#pragma once

#include <entt/entt.hpp>


#include "../behavior/DragonBehaviorSystem.h"
#include "../graphics/RendererSystem.h"

class SystemRunner
{
public:
	explicit SystemRunner(entt::registry& registry);

	void UpdateSystems() const;

private:
	RendererSystem renderSystem;
	DragonBehaviorSystem dragonBehaviorSystem;
};
