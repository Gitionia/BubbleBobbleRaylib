#include "EnemyProjectileBehaviorSystem.h"

#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"
#include "entt/entity/fwd.hpp"

static void makeProjectileDestroyed(entt::registry &registry, entt::entity e) {
    registry.emplace<EnemyProjectileInDestroyedAnimationTag>(e);
}

void EnemyProjectileBehaviorSystem::Init() {
}

void EnemyProjectileBehaviorSystem::Update() {

    const Collider &collider = Colliders::enemyProjectileCollider;

    auto view = registry.view<Position, EnemyProjectileComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, projectile, renderData] = view.get(entity);

        // Initializing starting position of witch projectile
        if (projectile.xPosStartPosition < -1000) {
            projectile.xPosStartPosition = pos.x;
        }

        projectile.animator.Update();
        if (projectile.animator.IsFinished()) {
            if (projectile.state == EnemyProjectileComponent::DESTROYED) {
                Destroy(entity);
                continue;
            }

            projectile.animator.Reset();
        }
        renderData.spriteHandle = projectile.animator.GetSpriteHandle();

        int maxDistance = BP_SIZE(1000, 0);
        int shootSpeed = 1;
        if (projectile.shooterType == EnemyType::GHOST) {
            shootSpeed = BP_SIZE(0, 3);
        } else if (projectile.shooterType == EnemyType::POTATO) {
            shootSpeed = BP_SIZE(0, 4);
        } else if (projectile.shooterType == EnemyType::WITCH) {
            shootSpeed = BP_SIZE(0, 5);
            maxDistance = BP_SIZE(14, 0);
        } else if (projectile.shooterType == EnemyType::SNOWMAN) {
            shootSpeed = BP_SIZE(0, 4);
        }

        int velocity = shootSpeed * pos.dir;

        // Reversing only happens for with projectiles (when they hit a wall or traveled maxDistance)
        if (projectile.state == EnemyProjectileComponent::SHOOTING_REVERSING) {
            pos.x -= velocity;
            // check if witch projectile reached its starting position and if so, destroy projectile
            if (pos.dir > 0 && pos.x <= projectile.xPosStartPosition || pos.dir < 0 && pos.x >= projectile.xPosStartPosition) {
                Destroy(entity);
            }

        // Normally travel forwards
        } else if (projectile.state != EnemyProjectileComponent::DESTROYED) {
            pos.x += velocity;
        }

        projectile.distanceMoved += shootSpeed;

        // Check if hit wall or traveled maxDistance
        if (((projectile.distanceMoved > maxDistance && projectile.state != EnemyProjectileComponent::SHOOTING_REVERSING) || collidesWithWall(registry, pos, collider)) && projectile.state != EnemyProjectileComponent::DESTROYED) {
            pos.x -= velocity;

            if (projectile.shooterType == EnemyType::GHOST) {
                projectile.animator.SetNewAnimation(&GetAnimation("Projectile-Ghost-Destroy"));
                projectile.state = EnemyProjectileComponent::DESTROYED;

                Defer(entity, &makeProjectileDestroyed, 0);

            } else if (projectile.shooterType == EnemyType::WITCH) {
                projectile.state = EnemyProjectileComponent::SHOOTING_REVERSING;
            }

            else {
                Destroy(entity);
                continue;
            }
        }
    }
}
