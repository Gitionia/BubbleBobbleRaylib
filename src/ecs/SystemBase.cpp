#include "SystemBase.h"

#include "Components.h"

SystemBase::SystemBase(entt::registry &registry)
    : registry(registry) {
}

void SystemBase::Destroy(const entt::entity &e) const {
    if (!registry.any_of<DestroyEntity>(e)) {
        registry.emplace<DestroyEntity>(e);
    }
}
