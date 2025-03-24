#include "Systems.h"


SystemRunner::SystemRunner(entt::registry &registry)
	: physicsSystem(registry), renderSystem(registry), dragonBehaviorSystem(registry)
{
}

void SystemRunner::UpdateSystems() const {
	physicsSystem.update();
	renderSystem.update();
}

