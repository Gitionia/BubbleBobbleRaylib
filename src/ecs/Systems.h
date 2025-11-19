#pragma once

#include <entt/entt.hpp>


#include "EntityFactory.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../behavior/BubbleBehaviorSystem.h"
#include "../graphics/RendererSystem.h"
#include "../graphics/Sprites.h"


class SystemRunner
{
public:
	explicit SystemRunner(entt::registry& registry);
	~SystemRunner();
	void Init();
	void UpdateSystems() const;


private:
	template<typename T>
	void registerSystem(entt::registry& registry);

private:
	entt::registry& registry;
	std::vector<SystemBase*> systems;
};
