#include "SystemBase.h"

#include "Components.h"


SystemBase::SystemBase(entt::registry &registry)
	: registry(registry)
{
}

void SystemBase::Destroy(const entt::entity& e) const {
	registry.emplace<DestroyEntity>(e);
}
