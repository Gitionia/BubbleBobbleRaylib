#include "ComponentUtils.h"

#include "Components.h"
#include "entt/entity/fwd.hpp"

void MakeBubbleAndGroupPopFromLifetime(entt::registry &registry, entt::entity entity) {

    // If this bubble was already popped, then we don't pop the group
    if (registry.any_of<BubblePopComponent>(entity)) {
        return;
    }

    if (registry.any_of<BubbleFloatComponent>(entity)) {
        BubbleFloatComponent floatComp = registry.get<BubbleFloatComponent>(entity);
        registry.remove<BubbleFloatComponent>(entity);

        BubblePopComponent &c = registry.emplace<BubblePopComponent>(entity);
        c.poppedFromLifeTime = true;

        // Pop all bubbles in same group as entity
        // Don't pop enemy bubbles!
        auto view = registry.view<BubbleFloatComponent>(entt::exclude<EnemyTag, BubblePopComponent>);
        for (entt::entity otherEntity : view) {
            BubbleFloatComponent otherFloatComp = registry.get<BubbleFloatComponent>(otherEntity);
            registry.remove<BubbleFloatComponent>(otherEntity);

            if (otherFloatComp.sharesLeaderWith(floatComp)) {
                BubblePopComponent &otherPopComponent = registry.emplace<BubblePopComponent>(otherEntity);
                otherPopComponent.poppedFromLifeTime = true;
            }
        }
    }
}

void MakeBubbleAndGroupPopFromDragonSpikes(entt::registry &registry, entt::entity entity) {

    // If this bubble was already popped, then we don't pop the group
    if (registry.any_of<BubblePopComponent>(entity)) {
        return;
    }

    if (registry.any_of<BubbleFloatComponent>(entity)) {
        BubbleFloatComponent floatComp = registry.get<BubbleFloatComponent>(entity);
        registry.remove<BubbleFloatComponent>(entity);

        int itemLevel = 0;
        BubblePopComponent &thisPopComponent = registry.emplace<BubblePopComponent>(entity);
        thisPopComponent.poppedFromLifeTime = false;

        if (registry.any_of<EnemyTag>(entity)) {
            thisPopComponent.itemLevel = itemLevel;
            itemLevel++;
        }

        // Pop all bubbles in same group as entity
        auto view = registry.view<BubbleFloatComponent>(entt::exclude<BubblePopComponent>);
        for (entt::entity otherEntity : view) {
            BubbleFloatComponent otherFloatComp = registry.get<BubbleFloatComponent>(otherEntity);
            registry.remove<BubbleFloatComponent>(otherEntity);

            if (otherFloatComp.sharesLeaderWith(floatComp)) {

                BubblePopComponent &otherPopComponent = registry.emplace<BubblePopComponent>(otherEntity);
                otherPopComponent.poppedFromLifeTime = false;

                if (registry.any_of<EnemyTag>(otherEntity)) {
                    otherPopComponent.itemLevel = itemLevel;
                    itemLevel++;
                }
            }
        }
    }
}
