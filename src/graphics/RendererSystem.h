#pragma once

#include "entt/entt.hpp"
#include "raylib.h"
#include "../ecs/Systems.h"

class RendererSystem {
	ECS_SYSTEM(RendererSystem)
public:
	~RendererSystem();

private:
	Texture2D mainSpriteSheet;
};