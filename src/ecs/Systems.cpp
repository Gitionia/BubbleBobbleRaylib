#include "Systems.h"

#include "Components.h"


SystemRunner::SystemRunner(entt::registry &registry, const SpriteManager& spriteManager)
	: registry(registry)
{
	registerSystem<RendererSystem>(registry, spriteManager);
	registerSystem<DragonBehaviorSystem>(registry, spriteManager);
	registerSystem<BubbleBehaviorSystem>(registry, spriteManager);
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

	auto view = registry.view<DestroyEntity>();
	registry.destroy(view.begin(), view.end());
}

template<typename T>
void SystemRunner::registerSystem(entt::registry& registry, const SpriteManager& spriteManager) {
	T* system = new T(registry, spriteManager);
	systems.push_back(system);
}

