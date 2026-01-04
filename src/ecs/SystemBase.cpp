#include "SystemBase.h"

#include "Components.h"

#include "../utils/Debug.h"

void SystemBase::BaseInit() {
    deferedEntityCollections.resize(DEFER_MAX_COUNT);
    for (auto vector : deferedEntityCollections) {
        vector.reserve(10);
    }

    deferFunctions.resize(DEFER_MAX_COUNT);
}

SystemBase::SystemBase(entt::registry &registry)
    : registry(registry) {
}

void SystemBase::Defer(entt::entity e, DeferFunctionType f, int index) {
    DBG_ASSERT(index >= 0 && index < DEFER_MAX_COUNT);

    deferedEntityCollections.at(index).push_back(e);
    deferFunctions.at(index) = f;
}

void SystemBase::BaseUpdate() {
    Update();

    auto view = registry.view<DestroyEntity>();
    registry.destroy(view.begin(), view.end());
    
    for (int i = 0; i < DEFER_MAX_COUNT; i++) {
        for (auto entity : deferedEntityCollections.at(i)) {
            DeferFunctionType& f = deferFunctions.at(i);

            f(registry, entity);
        }

        deferedEntityCollections.at(i).clear();
    }
}

void SystemBase::Destroy(const entt::entity &e) const {
    if (!registry.any_of<DestroyEntity>(e)) {
        registry.emplace<DestroyEntity>(e);
    }
}
