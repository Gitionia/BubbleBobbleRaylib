#pragma once

#include "entt/entt.hpp"
#include "raylib.h"

class RendererSystem {
public:

	explicit RendererSystem(entt::registry& registry);

	void update() const;

public:
	static constexpr int SCALE_SIZE = 2;
private:
	entt::registry& registry;
};