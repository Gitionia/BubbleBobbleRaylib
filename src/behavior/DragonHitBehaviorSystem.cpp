#include "DragonHitBehaviorSystem.h"

#include <iostream>
#include <iterator>

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../utils/Debug.h"
#include "../utils/Input.h"
#include "Level.h"
#include "Physics.h"
#include "WalkingActorUtils.h"
#include "entt/entity/fwd.hpp"

void makeDragonRespawn(entt::registry &registry, entt::entity e) {
    registry.remove<DragonHitComponent>(e);
    registry.get<WalkingActorComponent>(e).reset(DragonComponent::FALL_SPEED, DragonComponent::JUMP_SPEED);
    registry.get<DragonComponent>(e).reset();
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
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-HitStare"));
                    dragon.state = DragonHitComponent::HIT_STARE;
                    dragon.repetitionCount = 5;

                } else if (dragon.state == DragonHitComponent::HIT_STARE) {
                    dragon.animator.SetNewAnimation(&GetAnimation("Dragon-Respawn"));
                    dragon.state = DragonHitComponent::RESPAWN;
                    dragon.repetitionCount = 1;

                } else {
                    Defer(entity, &makeDragonRespawn, 0);
                }
            }
        }
    }
}
