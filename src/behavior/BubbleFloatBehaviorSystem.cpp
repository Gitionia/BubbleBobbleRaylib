#include "BubbleFloatBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../utils/Debug.h"
#include "../utils/Log.h"
#include "Physics.h"
#include "entt/entt.hpp"

static void makeBubblePop(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleFloatComponent>(entity);
    registry.emplace<BubblePopComponent>(entity);
}

void BubbleFloatBehaviorSystem::Update() {
    const Collider &col = Colliders::bubbleCollider;

    auto view = registry.view<Position, BubbleFloatComponent, RenderData>();
    for (auto entity : view) {
        auto [pos, bubble, renderData] = view.get(entity);

        if (bubble.animator.IsFinished())
            bubble.animator.Reset();
        renderData.spriteHandle = bubble.animator.GetSpriteHandle();
        bubble.animator.Update();

        Vector2Int centerPos = col.getCenter(pos.x, pos.y);
        Vector2Int airflowVelocity = getAirflowDirection(col, pos.toVector());

        pos.x += airflowVelocity.X;
        if (collidesWithWall(registry, pos, col)) {
            pos.x -= airflowVelocity.X;
        }

        pos.y += airflowVelocity.Y;
        if (collidesWithWall(registry, pos, col)) {
            pos.y -= airflowVelocity.Y;
        }

        if (pos.y >= BP_SIZE(LevelTilemap::HEIGHT + 1, 2)) {
            pos.y = BP_SIZE(-2, 0);
        } else if (pos.y < BP_SIZE(-2, -2)) {
            pos.y = BP_SIZE(LevelTilemap::HEIGHT, -2);
        }

        // Debug::DrawPoint(centerPos.X + airflowVelocity.X, centerPos.Y +
        // airflowVelocity.Y, 32, { 0, 122, 122, 180});

        if (bubble.lifetimeFrame > 0) {
            bubble.lifetimeFrame--;
        }

        if (bubble.lifetimeFrame == 0 || collidesWithDragonSpikes(registry, pos, col)) {
            Defer(entity, &makeBubblePop, 0);
        }
    }
}
