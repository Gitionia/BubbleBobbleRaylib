#pragma once

#include <entt/entt.hpp>

#include "Components.h"
#include "../graphics/Sprites.h"

class EntityFactory {
public:
    EntityFactory(entt::registry& registry, const SpriteManager& spriteManager);

public:
    entt::entity CreateDragon() const;
    entt::entity CreateBubble(const Position &pos, int direction) const;

private:
    const SpriteManager& spriteManager;
	entt::registry& registry;
};