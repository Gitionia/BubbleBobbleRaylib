#include "ComponentUtils.h"

#include "../audio/Audio.h"
#include "Components.h"
#include "entt/entity/fwd.hpp"

static const Sound *getEnemyBubblePopSound(int index) {
    static std::array<const Sound *, 3> sounds{};
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < sounds.size(); i++) {
            sounds.at(i) = &GetSound(std::format("enemy-bubble-pop-level-{}", i + 1));
        }
        initialized = true;
    }

    return sounds.at(index);
}

static int choosePopSound(int poppedCount) {
    if (poppedCount <= 1) {
        return 0;
    } else if (poppedCount <= 4) {
        return 1;
    } else {
        return 2;
    }
}

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

            if (otherFloatComp.leader.sharesLeaderWith(floatComp.leader)) {
                // TODO: removing component here technically not safe!
                registry.remove<BubbleFloatComponent>(otherEntity);
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

            if (otherFloatComp.leader.sharesLeaderWith(floatComp.leader)) {

                // TODO: removing component here technically not safe!
                registry.remove<BubbleFloatComponent>(otherEntity);
                BubblePopComponent &otherPopComponent = registry.emplace<BubblePopComponent>(otherEntity);
                otherPopComponent.poppedFromLifeTime = false;

                if (registry.any_of<EnemyTag>(otherEntity)) {
                    otherPopComponent.itemLevel = itemLevel;
                    itemLevel++;
                }
            }
        }

        if (itemLevel > 0) {
            PlaySound(*getEnemyBubblePopSound(choosePopSound(itemLevel)));
        }
    }
}
