#include "Systems.h"

#include "Components.h"
#include "EntityFactory.h"


SystemRunner::SystemRunner(entt::registry &registry, const EntityFactory& factory, const SpriteManager& spriteManager)
	: registry(registry)
{
	registerSystem<RendererSystem>(registry, factory, spriteManager);
	registerSystem<DragonBehaviorSystem>(registry, factory, spriteManager);
	registerSystem<BubbleBehaviorSystem>(registry, factory, spriteManager);
}

SystemRunner::~SystemRunner() {
	for (auto system: systems) {
		delete system;
	}
}

void SystemRunner::Init() {
	for (auto system: systems) {
		system->Init();
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
void SystemRunner::registerSystem(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager) {
	T* system = new T(registry, factory, spriteManager);
	systems.push_back(system);
}

