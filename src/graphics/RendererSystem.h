#pragma once

#include "entt/entt.hpp"
#include "raylib.h"

class RendererSystem {
public:

	explicit RendererSystem(entt::registry& registry);
	~RendererSystem();

	void setRegistry(entt::registry& registry);
	void update() const;

	void init();

private:
	entt::registry& registry;
private:
	Texture2D mainSpriteSheet;
};