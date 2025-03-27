#include "Systems.h"


SystemRunner::SystemRunner(entt::registry &registry, const SpriteManager& spriteManager)
{
	registerSystem<RendererSystem>(registry, spriteManager);
	registerSystem<DragonBehaviorSystem>(registry, spriteManager);
}

SystemRunner::~SystemRunner() {
	for (auto system: systems) {
		delete system;
	}
}

void SystemRunner::UpdateSystems() const {
	for (auto system: systems) {
		system->Update();
	}
}

template<typename T>
void SystemRunner::registerSystem(entt::registry& registry, const SpriteManager& spriteManager) {
	T* system = new T(registry, spriteManager);
	systems.push_back(system);
}

