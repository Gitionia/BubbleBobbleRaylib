#include "Systems.h"

#include "../physics/PhysicsSystem.h"
#include "../graphics/RendererSystem.h"

PhysicsSystem physicsSystem;
RendererSystem renderSystem;

void initECSSystems(entt::registry& registry) {
	physicsSystem.setRegistry(registry);
	renderSystem.setRegistry(registry);
}

void updateECSSystems() {
	physicsSystem.update();
	renderSystem.update();
}
