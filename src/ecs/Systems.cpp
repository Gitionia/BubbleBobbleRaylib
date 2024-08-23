#include "Systems.h"


SystemRunner::SystemRunner(entt::registry &registry)
	: physicsSystem(registry), renderSystem(registry)
{
}

void SystemRunner::UpdateSystems() const {
	physicsSystem.update();
	renderSystem.update();
}

