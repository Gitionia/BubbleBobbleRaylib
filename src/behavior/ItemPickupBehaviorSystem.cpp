#include "ItemPickupBehaviorSystem.h"
#include "../level/Physics.h"
#include <memory>

void ItemPickupBehaviorSystem::Update() {
    auto dragonView = registry.view<Position, DragonTag>(entt::exclude<DragonHitComponent>);
    auto itemView = registry.view<Position, ItemComponent>(entt::exclude<DragonHitComponent>);

    for (auto dragon : dragonView) {
        auto [dragonPos] = dragonView.get(dragon);

        for (auto item : itemView) {
            auto [itemPos, itemComp] = itemView.get(item);

            if (overlaps(dragonPos, Colliders::fullActorCollider, 
                itemPos, Colliders::fullActorCollider)) {
                EntityFactory::CreateItemPointsText(itemPos.toVector(), itemComp.type);
                Destroy(item);

                eventSystem.Notify(dragon, POINTS_GAINED, GetItemPoints(itemComp.type));
            }
        }
    }
}