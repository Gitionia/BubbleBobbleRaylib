#pragma once

#include <entt/entt.hpp>


#include "../behavior/DragonBehaviorSystem.h"
#include "../behavior/BubbleBehaviorSystem.h"
#include "../graphics/RendererSystem.h"
#include "../graphics/Sprites.h"


class SystemRunner
{
public:
	explicit SystemRunner(entt::registry& registry, const SpriteManager& spriteManager);
	~SystemRunner();
	void UpdateSystems() const;

private:
	template<typename T>
	void registerSystem(entt::registry& registry, const SpriteManager& spriteManager);

private:
	std::vector<SystemBase*> systems;
};
