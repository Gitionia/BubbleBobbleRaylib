#include "DragonHitBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"
#include "entt/entity/fwd.hpp"

static void makeNewGreenDragon(entt::registry &registry, entt::entity e) {
    EntityFactory::CreateDragon(DRAGON_GREEN, true);
}

static void makeNewBlueDragon(entt::registry &registry, entt::entity e) {
    EntityFactory::CreateDragon(DRAGON_BLUE, true);
}

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
                    dragon.animator.SetNewAnimation(&GetAnimation(GetDragonAnimation(DragonAnimationType::HIT_STARE, dragon.color)));
                    dragon.state = DragonHitComponent::HIT_STARE;
                    dragon.repetitionCount = 5;

                } else if (dragon.state == DragonHitComponent::HIT_STARE) {
                    dragon.animator.SetNewAnimation(&GetAnimation(GetDragonAnimation(DragonAnimationType::RESPAWN, dragon.color)));
                    dragon.state = DragonHitComponent::RESPAWN;
                    dragon.repetitionCount = 1;

                } else {
                    Destroy(entity);

                    if (dragon.color == DRAGON_GREEN) {
                        Defer((entt::entity)0, &makeNewGreenDragon, 0);
                    } else {
                        Defer((entt::entity)0, &makeNewBlueDragon, 0);
                    }
                    eventSystem.Notify(entity, PLAYER_DIED, 0);
                }
            }
        }
    }
}
