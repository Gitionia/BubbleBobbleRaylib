#pragma once

#include <entt/entt.hpp>

#include "Components.h"
#include "../behavior/Level.h"
#include "../graphics/Sprites.h"


class EntityFactory {
public:
	static entt::entity CreateTile(int x, int y, Color shadowRight, Color shadowBottem, bool addShadowRight = true, bool addShadowBottem = true);
	static entt::entity CreateDragon();
	static entt::entity CreateBubble(const Position &pos, int direction);
	static void CreateLevel(const LevelLayout & level);

private:
    static EntityFactory& get();
    void setRegistry(entt::registry& registry);
	entt::registry* registry;

	friend class Application;
};
