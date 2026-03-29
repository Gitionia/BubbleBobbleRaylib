#include "DragonHitBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"

void DragonHitBehaviorSystem::Init() {
}

void DragonHitBehaviorSystem::Update() {

    auto view = registry.view<Position, DragonHitComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, dragon, renderData] = view.get(entity);

        pos.dir = -1;
        renderData.spriteHandle = dragon.animator.GetSpriteHandle();
        dragon.animator.Update();

        if (dragon.animator.IsFinished()) {

            dragon.repetitionCount--;
            dragon.animator.Reset();

            if (dragon.repetitionCount == 0) {
                if (dragon.state == DragonHitComponent::HIT) {
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-HitStare"));
                    dragon.state = DragonHitComponent::HIT_STARE;
                    dragon.repetitionCount = 5;

                } else if (dragon.state == DragonHitComponent::HIT_STARE) {
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-Respawn"));
                    dragon.state = DragonHitComponent::RESPAWN;
                    dragon.repetitionCount = 1;

                } else {
                    Destroy(entity);
                    EntityFactory::CreateDragon();
                }
            }
        }
    }
}
