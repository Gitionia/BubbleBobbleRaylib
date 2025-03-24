#pragma once

#include "entt/entt.hpp"

class DragonBehaviorSystem {
public:
	explicit DragonBehaviorSystem(entt::registry& registry);

	void Update() const;

private:
	entt::registry& registry;
};
