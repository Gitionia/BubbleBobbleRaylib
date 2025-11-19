#include "SystemBase.h"

#include "Components.h"


SystemBase::SystemBase(entt::registry &registry, const EntityFactory& factory)
	: registry(registry), factory(factory)
{
}

void SystemBase::Destroy(const entt::entity& e) const {
	registry.emplace<DestroyEntity>(e);
}
