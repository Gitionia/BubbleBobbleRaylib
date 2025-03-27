#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entt.hpp"

class DragonBehaviorSystem : public SystemBase{
	SYSTEM_DEF(DragonBehaviorSystem)

public:
	void Update() override;
};
