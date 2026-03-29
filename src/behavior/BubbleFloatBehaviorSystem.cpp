#include "BubbleFloatBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../level/Physics.h"

enum DeferValues {
    PopFromLifeTime = 0,
    PopFromDragonSpikes = 1
};

static void makeBubblePopFromLifeTime(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleFloatComponent>(entity);
    BubblePopComponent &c = registry.emplace<BubblePopComponent>(entity);
    c.poppedFromLifeTime = true;
}

static void makeBubblePopFromDragonSpikes(entt::registry &registry, entt::entity entity) {

    registry.remove<BubbleFloatComponent>(entity);
    BubblePopComponent &c = registry.emplace<BubblePopComponent>(entity);
    c.poppedFromLifeTime = false;
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
        Vector2Int bubbleRepelDirection = getBubbleRepelVelocity(registry, Colliders::bubbleRepelCollider, pos.toVector());
        const int BUBBLE_REPEL_SPEED = 2;
        Vector2Int velocity = airflowVelocity.Add(BUBBLE_REPEL_SPEED * bubbleRepelDirection.X, BUBBLE_REPEL_SPEED * bubbleRepelDirection.Y);

        if (bubble.isWaitingForPop()) {
            bubble.popFrame--;
            if (bubble.popFrame == 0 || collidesWithDragonSpikes(registry, pos, col)) {
                // Note: this should only be reached by actual bubbles and not bubbled enemies,
                // so we don't care that much, how we set popComponent.poppedFromLifeTime
                Defer(entity, &makeBubblePopFromDragonSpikes, PopFromDragonSpikes);
            }

        } else {
            pos.x += velocity.X;
            if (collidesWithWall(registry, pos, col)) {
                pos.x -= velocity.X;
            }

            pos.y += velocity.Y;
            if (collidesWithWall(registry, pos, col)) {
                pos.y -= velocity.Y;
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

            if (bubble.lifetimeFrame == 0) {
                Defer(entity, &makeBubblePopFromLifeTime, PopFromLifeTime);
            }
            if (collidesWithDragonSpikes(registry, pos, col)) {
                Defer(entity, &makeBubblePopFromDragonSpikes, PopFromDragonSpikes);
            }
        }

        Debug::DrawCollider(pos.x, pos.y, Colliders::bubbleRepelCollider);
    }
}
