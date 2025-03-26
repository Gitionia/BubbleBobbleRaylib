#include "Systems.h"


SystemRunner::SystemRunner(entt::registry &registry)
	: renderSystem(registry), dragonBehaviorSystem(registry)
{
}

void SystemRunner::UpdateSystems() const {
	renderSystem.Update();
	dragonBehaviorSystem.Update();
}

