#pragma once

#include "entt/entt.hpp"
#include "raylib.h"
#include "../ecs/SystemBase.h"

class RendererSystem : public SystemBase {
	SYSTEM_DEF(RendererSystem)

public:
	void Update() override;

public:
	static constexpr int SCALE_SIZE = 2;
};