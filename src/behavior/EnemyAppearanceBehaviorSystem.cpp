#include "EnemyAppearanceBehaviorSystem.h"
#include "entt/entity/fwd.hpp"

void removeAppearanceComponent(entt::registry& registry, entt::entity e) {
    registry.remove<EnemyAppearanceComponent>(e);
}

void EnemyAppearanceBehaviorSystem::Update() {

    auto view = registry.view<Position, EnemyAppearanceComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, appearance, renderData] = view.get(entity);
    
        appearance.animator.Update();
        if (appearance.animator.IsFinished()) {
            appearance.animator.Reset();
        }
        renderData.spriteHandle = appearance.animator.GetSpriteHandle();


        if (appearance.yOffset < 0) {
            appearance.yOffset += BP_SIZE(0, 2);
        
        } else if (appearance.waitingDelay > 0) {
            appearance.waitingDelay--;
        
        } else {
            Defer(entity, &removeAppearanceComponent, 0);
            pos.x = appearance.targetPosX;
            pos.y = appearance.targetPosY;
            continue;
        }

        // PRINT_INFO("offset {} delay{}", appearance.yOffset, appearance.waitingDelay);
        pos.x = appearance.targetPosX;
        pos.y = appearance.targetPosY + appearance.yOffset;
    }
}
