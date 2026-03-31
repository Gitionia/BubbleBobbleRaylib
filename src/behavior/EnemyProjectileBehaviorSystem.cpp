#include "EnemyProjectileBehaviorSystem.h"

#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"
#include "entt/entity/fwd.hpp"

static void makeProjectileDestroyed(entt::registry& registry, entt::entity e) {
    registry.emplace<EnemyProjectileInDestroyedAnimationTag>(e);
}

void EnemyProjectileBehaviorSystem::Init() {
}

void EnemyProjectileBehaviorSystem::Update() {

    const Collider &collider = Colliders::enemyProjectileCollider;

    auto view = registry.view<Position, EnemyProjectileComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, projectile, renderData] = view.get(entity);

        projectile.animator.Update();
        if (projectile.animator.IsFinished()) {
            if (projectile.state == EnemyProjectileComponent::DESTROYED) {
                Destroy(entity);
                continue;
            }

            projectile.animator.Reset();
        }
        renderData.spriteHandle = projectile.animator.GetSpriteHandle();



        const int shootSpeed = BP_SIZE(0, 3);
        int velocity = shootSpeed * pos.dir;

        if (projectile.state != EnemyProjectileComponent::DESTROYED) {
            pos.x += velocity;
        }

        if (collidesWithWall(registry, pos, collider) && projectile.state != EnemyProjectileComponent::DESTROYED) {
            pos.x -= velocity;

            if (projectile.shooterType == EnemyType::GHOST) {
                projectile.animator.SetNewAnimation(&GetAnimation("Projectile-Ghost-Destroy"));
                projectile.state = EnemyProjectileComponent::DESTROYED;

                Defer(entity, &makeProjectileDestroyed, 0);
            } 
            else {
                Destroy(entity);
                continue;
            }
        }
    }
}
