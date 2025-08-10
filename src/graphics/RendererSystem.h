#pragma once

#include "entt/entt.hpp"
#include "raylib.h"
#include "../ecs/SystemBase.h"
#include "../app/Config.h"

class RendererSystem : public SystemBase {
	SYSTEM_DEF(RendererSystem)

public:
	void Update() override;

private:
	template<typename Tag>
	void renderAllWithTag();

	template<typename ColliderType>
    void debugDrawColliders(Color color);
};