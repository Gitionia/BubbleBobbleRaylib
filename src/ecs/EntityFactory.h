#pragma once

#include <entt/entt.hpp>
#include "../graphics/Sprites.h"

class EntityFactory {
public:
    EntityFactory(entt::registry& registry, const SpriteManager& spriteManager);

public:
    entt::entity createDragon();

private:
    const SpriteManager& spriteManager;
	entt::registry& registry;
};