#include "BubblePopBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../level/Physics.h"
#include "entt/entity/fwd.hpp"

struct ItemLevelForEntity {
    entt::entity entity;
    int itemLevel;
};

static std::vector<ItemLevelForEntity> itemLevels{};

void BubblePopBehaviorSystem::Init() {
    itemLevels.reserve(20);
}

void makeBubblePop(entt::registry &registry, entt::entity e) {

    registry.remove<BubbleFloatComponent>(e);
    if (!registry.any_of<BubblePopComponent>(e)) {

        BubblePopComponent &c = registry.emplace<BubblePopComponent>(e);
        c.poppedFromLifeTime = false;

        int itemLevel = 0;
        for (const ItemLevelForEntity& itemLevelForEntity : itemLevels) {
            if (itemLevelForEntity.entity == e && itemLevelForEntity.itemLevel > itemLevel) {
                itemLevel = itemLevelForEntity.itemLevel;
            }
        }

        DBG_CHECK(itemLevel > 0, "Item Level for popped bubble was not > 0, even though it was popped by other bubble");
        c.itemLevel = itemLevel;

    }
}

void BubblePopBehaviorSystem::popAdjacentBubbles(const Position &position, const BubblePopComponent &popComp) {
    auto view = registry.view<Position, BubbleFloatComponent>();

    for (entt::entity entity : view) {
        auto [pos, bubble] = view.get(entity);

        if (overlaps(position, Colliders::bubblePopCollider, pos, Colliders::bubblePopCollider)) {

            Defer(entity, &makeBubblePop, 0);
            itemLevels.push_back({entity, popComp.itemLevel + 1});
        }
    }
}

void BubblePopBehaviorSystem::Update() {
    itemLevels.clear();

    auto view = registry.view<Position, BubblePopComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, bubble, renderData] = view.get(entity);

        int MAX_POP_ANIM_REPETITIONS = 2;

        if (!bubble.gavePointsForPop) {
            // We check this only once for this entity,
            // so we say in any case that points were given
            bubble.gavePointsForPop = true;

            if (!registry.any_of<EnemyTag>(entity)) {
                eventSystem.Notify(entity, POINTS_GAINED, 10);
            }
        }

        pos.dir = -1;
        if (bubble.isInStatePrePop) {

            if (!bubble.poppedFromLifeTime) {
                popAdjacentBubbles(pos, bubble);
            }

            renderData.spriteHandle = bubble.animator.GetSpriteHandle();
            bubble.animator.Update();

            if (bubble.animator.IsFinished()) {
                bubble.isInStatePrePop = false;
                bubble.animator.SetNewAnimation(&GetAnimation("Bubble-Pop"));
            }
        } else {
            renderData.spriteHandle = bubble.animator.GetSpriteHandle();

            renderData.color = {255, 240, 215, 255};
            if (!bubble.animator.IsFinished()) {
                bubble.animator.Update();

            } else {

                bubble.animator.Reset();
                bubble.popAnimationRepetitions++;

                if (bubble.popAnimationRepetitions == MAX_POP_ANIM_REPETITIONS) {
                    Destroy(entity);
                }
            }
        }
    }
}
