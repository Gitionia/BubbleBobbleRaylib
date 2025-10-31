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
	explicit SystemRunner(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager);
	~SystemRunner();
	void UpdateSystems() const;

private:
	template<typename T>
	void registerSystem(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager);

private:
	entt::registry& registry;
	std::vector<SystemBase*> systems;
};
