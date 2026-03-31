#include "EnemyProjectileBehaviorSystem.h"

#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"
#include "WalkingActorUtils.h"

void EnemyProjectileBehaviorSystem::Init() {
}

void EnemyProjectileBehaviorSystem::Update() {

    const Collider &collider = Colliders::projectileCollider;

    auto view = registry.view<Position, EnemyProjectileComponent>();
    for (auto entity : view) {
        auto [pos, projectile] = view.get(entity);

        const int shootVelocity = BP_SIZE(0, 2);

        pos.x += shootVelocity * pos.dir;

        if (collidesWithWall(registry, pos, collider)) {
            Destroy(entity);
        }
    }
}
