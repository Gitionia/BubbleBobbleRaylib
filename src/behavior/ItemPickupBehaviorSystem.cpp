#include "ItemPickupBehaviorSystem.h"
#include "../level/GameModifiers.h"
#include "../level/Physics.h"
#include <cstdint>

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

                // for testing every item will give this effect for now
                if (itemComp.type == ItemType::Shoe) {
                    eventSystem.Notify(item, USING_GAME_MODIFIER, (uint64_t)ModifierTypes::SPEED_UP);
                
                } else if (itemComp.type == ItemType::Book) {
                    eventSystem.Notify(item, STORY_ITEM_PICKED_UP, (int64_t)itemComp.type);
                }
            }
        }
    }
}