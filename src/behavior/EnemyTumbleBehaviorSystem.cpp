#include "EnemyTumbleBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "Level.h"
#include "Physics.h"
#include "WalkingActorUtils.h"

void EnemyTumbleBehaviorSystem::Init() {
}

void EnemyTumbleBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, EnemyTumbleComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, enemy, renderData] = view.get(entity);

        pos.x += enemy.xVel.get() * pos.dir;
        pos.y += -enemy.yVel.get();
        if (pos.x < 2 * UNITS_PER_BLOCK) {
            pos.x = 2 * UNITS_PER_BLOCK;
            pos.dir = 1; 
        } else if (pos.x > LevelTilemap::WIDTH * UNITS_PER_BLOCK) {
            pos.x = LevelTilemap::WIDTH * UNITS_PER_BLOCK;
            pos.dir = -1; 
        }

        
        enemy.xVel.tick();
        enemy.yVel.tick();

        enemy.animator.Update();
        if (enemy.animator.IsFinished()) {
            enemy.animator.Reset();
        }

        renderData.spriteHandle = enemy.animator.GetSpriteHandle();
    }
}
