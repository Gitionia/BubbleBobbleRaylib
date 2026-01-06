#include "PopEnemyBubbleSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "Physics.h"
#include "WalkingActorUtils.h"


void PopEnemyBubbleSystem::Update() {

    auto view = registry.view<BubblePopComponent, EnemyTag>();
    for (auto entity : view) {
        auto [pop] = view.get(entity);
    
        if (pop.poppedFromLifeTime) {
            // revert enemy to normal enemy
        } else {
            // move enemy into dice state
        }
    }
}
