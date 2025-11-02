#pragma once

#include <entt/entt.hpp>


#include "EntityFactory.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../behavior/BubbleBehaviorSystem.h"
#include "../graphics/RendererSystem.h"
#include "../graphics/Sprites.h"
#include "../audio/AudioManager.h"


class SystemRunner
{
public:
	explicit SystemRunner(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager, const AudioManager& audio);
	~SystemRunner();
	void UpdateSystems() const;

private:
	template<typename T>
	void registerSystem(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager, const AudioManager& audio);

private:
	entt::registry& registry;
	std::vector<SystemBase*> systems;
};
