#include "ItemPickupBehaviorSystem.h"
#include "Physics.h"
#include <memory>

void ItemPickupBehaviorSystem::Update() {
    auto dragonView = registry.view<Position, DragonTag>(entt::exclude<DragonHitComponent>);
    auto itemView = registry.view<Position, ItemTag>(entt::exclude<DragonHitComponent>);

    for (auto dragon : dragonView) {
        auto [dragonPos] = dragonView.get(dragon);

        for (auto item : itemView) {
            auto [itemPos] = itemView.get(item);

            if (overlaps(dragonPos, Colliders::fullActorCollider, 
                itemPos, Colliders::fullActorCollider)) {
                EntityFactory::CreateItemPointsText(itemPos.toVector());
                Destroy(item);

                eventSystem.Notify(dragon, POINTS_GAINED, 500);
            }
        }
    }
}