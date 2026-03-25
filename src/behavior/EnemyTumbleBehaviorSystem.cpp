#include "EnemyTumbleBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"

void EnemyTumbleBehaviorSystem::Init() {
}

void EnemyTumbleBehaviorSystem::Update() {

    const Collider &collider = Colliders::walkingActorCollider;

    auto view = registry.view<Position, EnemyTumbleComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, enemy, renderData] = view.get(entity);

        if (!enemy.isFalling) {
            pos.x += enemy.xVel.get() * pos.dir;
        }

        pos.y += -enemy.yVel.get();
        if (pos.x < 2 * UNITS_PER_BLOCK) {
            pos.x = 2 * UNITS_PER_BLOCK;
            pos.dir = 1; 
        } else if (pos.x > LevelTilemap::WIDTH * UNITS_PER_BLOCK) {
            pos.x = LevelTilemap::WIDTH * UNITS_PER_BLOCK;
            pos.dir = -1; 
        }

        int BOTTEM_WARP_POS = 27 * UNITS_PER_BLOCK;
        // apply warping
        if (pos.y >= BOTTEM_WARP_POS) {
            pos.y = BP_SIZE(-3, 0);
        } else if (pos.y < BP_SIZE(-3, -2)) {
            pos.y = BP_SIZE(LevelTilemap::HEIGHT + 1, 0);
        }


        if (enemy.xVel.onLastFrame() || enemy.yVel.onLastFrame()) {
            enemy.isFalling = true;

            if (enemy.ignoreCollision) {
                enemy.ignoreCollision = shouldWalkingActorIgnoreCollisions(registry, pos, Colliders::fullActorCollider);
            }
        }

        if (!enemy.isFalling) {
            enemy.xVel.tick();
            enemy.yVel.tick();
        
        } else {
            WalkingActorComponent dummyComponent {
                .fallSpeed = enemy.yVel.get(),
                .jumpSpeed = 0,
                .ignoreCollisions = enemy.ignoreCollision,
                .jumpFrameCount = 0
            };

            if (isWalkingActorGrounded(registry, pos, dummyComponent)) {
                // pos.y will overshoot desired y-position on a block
                pos.y = (pos.y / UNITS_PER_BLOCK) * UNITS_PER_BLOCK;
                
                EntityFactory::CreateItem(pos.toVector());
                Destroy(entity);
                continue;
            }

        }


        enemy.animator.Update();
        if (enemy.animator.IsFinished()) {
            enemy.animator.Reset();
        }

        renderData.spriteHandle = enemy.animator.GetSpriteHandle();
    }
}
