#include "Systems.h"

#include "Components.h"
#include "EntityFactory.h"
#include "../audio/AudioSystem.h"


SystemRunner::SystemRunner(entt::registry &registry, const EntityFactory& factory, const SpriteManager& spriteManager, const AudioManager& audio)
	: registry(registry)
{
	registerSystem<RendererSystem>(registry, factory, spriteManager, audio);
	registerSystem<DragonBehaviorSystem>(registry, factory, spriteManager, audio);
	registerSystem<BubbleBehaviorSystem>(registry, factory, spriteManager, audio);
	registerSystem<AudioSystem>(registry, factory, spriteManager, audio);
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
void SystemRunner::registerSystem(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager, const AudioManager& audio) {
	T* system = new T(registry, factory, spriteManager, audio);
	systems.push_back(system);
}

