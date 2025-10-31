#include "SystemBase.h"

#include "Components.h"


SystemBase::SystemBase(entt::registry &registry, const SpriteManager &spriteManager)
	: registry(registry), spriteManager(spriteManager)
{
}

void SystemBase::Destroy(const entt::entity& e) const {
	registry.emplace<DestroyEntity>(e);
}
