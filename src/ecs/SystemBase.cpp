#include "SystemBase.h"

#include "Components.h"


SystemBase::SystemBase(entt::registry &registry, const EntityFactory& factory, const SpriteManager &spriteManager, const AudioManager& audio)
	: registry(registry), factory(factory), spriteManager(spriteManager), audio(audio)
{
}

void SystemBase::Destroy(const entt::entity& e) const {
	registry.emplace<DestroyEntity>(e);
}
