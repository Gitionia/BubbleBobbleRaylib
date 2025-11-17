#pragma once

#include <entt/entt.hpp>

#include "Components.h"
#include "../behavior/Level.h"
#include "../graphics/Sprites.h"


class EntityFactory {
public:
    EntityFactory(entt::registry& registry, const SpriteManager& spriteManager);

public:
    entt::entity CreateTile(int x, int y, Color shadowRight, Color shadowBottem, bool addShadowRight = true, bool addShadowBottem = true) const;
    entt::entity CreateDragon() const;
    entt::entity CreateBubble(const Position &pos, int direction) const;

    void CreateLevel(LevelLayout & level) const;

private:
	entt::registry& registry;
    const SpriteManager& spriteManager;
};
