#include "ItemPickupBehaviorSystem.h"
#include "../audio/Audio.h"
#include "../level/GameModifiers.h"
#include "../level/Physics.h"
#include "raylib.h"
#include <cstdint>

const Sound *ItemPickupBehaviorSystem::getRandomItemPickupSound() {
    int i = Random::Get().Range(0,2);
    return itemPickupSounds.at(i);
}

void ItemPickupBehaviorSystem::Init() {
    for (int i = 0; i < itemPickupSounds.size(); i++) {
        itemPickupSounds.at(i) = &GetSound(std::format("item-pickup-{}", i + 1));
    }
}

void ItemPickupBehaviorSystem::Update() {
    auto dragonView = registry.view<Position, DragonTag>(entt::exclude<DragonHitComponent>);
    auto itemView = registry.view<Position, ItemComponent>(entt::exclude<DragonHitComponent>);

    for (auto dragon : dragonView) {
        auto [dragonPos] = dragonView.get(dragon);

        for (auto item : itemView) {
            auto [itemPos, itemComp] = itemView.get(item);

            if (overlaps(dragonPos, Colliders::fullActorCollider,
                         itemPos, Colliders::fullActorCollider)) {

                Destroy(item);
                if (GetItemPoints(itemComp.type) > 0) {
                    EntityFactory::CreateItemPointsText(itemPos.toVector(), itemComp.type);
                }

                PlaySound(*getRandomItemPickupSound());

                eventSystem.Notify(dragon, POINTS_GAINED, GetItemPoints(itemComp.type));

                if (itemComp.type == ItemType::Shoe) {
                    eventSystem.Notify(item, USING_GAME_MODIFIER, (uint64_t)ModifierTypes::SPEED_UP);

                } else if (itemComp.type == ItemType::Soup) {
                    eventSystem.Notify(item, USING_GAME_MODIFIER, (uint64_t)ModifierTypes::FIRERATE_UP);

                } else if (itemComp.type == ItemType::Meal) {
                    eventSystem.Notify(item, USING_GAME_MODIFIER, (uint64_t)ModifierTypes::RANGE_UP);

                } else if (itemComp.type == ItemType::ToyFlamingo) {
                    eventSystem.Notify(item, USING_GAME_MODIFIER, (uint64_t)ModifierTypes::SPEED_UP);
                    eventSystem.Notify(item, USING_GAME_MODIFIER, (uint64_t)ModifierTypes::FIRERATE_UP);
                    eventSystem.Notify(item, USING_GAME_MODIFIER, (uint64_t)ModifierTypes::RANGE_UP);

                } else if (itemComp.type == ItemType::Door || itemComp.type == ItemType::Book) {
                    eventSystem.Notify(item, STORY_ITEM_PICKED_UP, (int64_t)itemComp.type);
                }
            }
        }
    }
}