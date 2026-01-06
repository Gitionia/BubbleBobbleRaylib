#include "BubblePopBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "Physics.h"

void BubblePopBehaviorSystem::Update() {
    auto view = registry.view<Position, BubblePopComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, bubble, renderData] = view.get(entity);

        int MAX_POP_ANIM_REPETITIONS = 2;

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
