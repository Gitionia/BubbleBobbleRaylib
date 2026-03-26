#include "SystemBase.h"

#include "Components.h"
#include "entt/entity/fwd.hpp"

void SystemBase::BaseInit() {
    for (auto vector : deferedEntityCollections) {
        vector.reserve(20);
    }
}

SystemBase::SystemBase(entt::registry &registry, EventSystem& eventSystem, int typeFlags)
    : registry(registry), eventSystem(eventSystem), typeFlags(typeFlags) {
}

void SystemBase::Defer(entt::entity e, DeferFunctionType f, int index) {
    DBG_ASSERT(index >= 0 && index < DEFER_MAX_COUNT);

    deferedEntityCollections.at(index).push_back(e);
    deferFunctions.at(index) = f;
}

void SystemBase::BaseUpdate() {
    if (!enabled) return;

    Update();

    auto view = registry.view<DestroyEntity>();
    registry.destroy(view.begin(), view.end());
    
    for (int i = 0; i < DEFER_MAX_COUNT; i++) {
        std::vector<entt::entity>& entities = deferedEntityCollections.at(i);
        DeferFunctionType& f = deferFunctions.at(i);

        for (auto entity : entities) {
            DBG_ASSERT(f);
            f(registry, entity);
        }

        // clean up the defered data, because it should only be used for the current system
        f = nullptr;
        entities.clear();
    }
}

void SystemBase::Destroy(const entt::entity &e) const {
    if (!registry.any_of<DestroyEntity>(e)) {
        registry.emplace<DestroyEntity>(e);
    }
}

void SystemBase::SetEnabledIfMatchesAnyFlag(int flags) {
    enabled = this->typeFlags & flags;
}
