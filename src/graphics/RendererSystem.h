#pragma once

#include "entt/entt.hpp"
#include "raylib.h"

class RendererSystem {
public:

	explicit RendererSystem(entt::registry& registry);

	void update() const;

private:
	entt::registry& registry;
};