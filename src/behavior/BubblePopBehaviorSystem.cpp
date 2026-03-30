#include "BubblePopBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../level/Physics.h"
#include "entt/entity/fwd.hpp"


void BubblePopBehaviorSystem::Update() {
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
