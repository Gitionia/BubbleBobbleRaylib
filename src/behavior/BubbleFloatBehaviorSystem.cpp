#include "BubbleFloatBehaviorSystem.h"

#include "../app/Config.h"
#include "../ecs/ComponentUtils.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../level/Physics.h"
#include <algorithm>

enum DeferValues {
    PopFromLifeTime = 0,
    PopFromDragonSpikes = 1
};


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
        int dragonBubblePushSpeed = BP_SIZE(0, 1);
        int dragonBubblePushVelocity = dragonBubblePushSpeed * getDragonBubblePushDirection(registry, pos, col);

        // calculate repel direction and update group leader
        Vector2Int bubbleRepelDirection = Vector2Int::Zero();
        {
            if (!bubble.leader.areGroupLeaderInitialized()) {
                bubble.leader.initGroupLeader((int)entity);
            }
            if (bubble.leader.updateLeaderSwitchCounterAndSwitch()) {
                bubble.leader.setLeader((int)entity);
            }

            const auto innerBubbleView = registry.view<Position, BubbleFloatComponent>();

            for (entt::entity otherEntity : innerBubbleView) {
                // we don't check this bubble against itself
                if (entity == otherEntity)
                    continue;

                const auto [otherPos, otherBubble] = innerBubbleView.get(otherEntity);

                if (otherBubble.leader.areGroupLeaderInitialized() && overlaps(pos, Colliders::bubblePopCollider, otherPos, Colliders::bubblePopCollider)) {

                    bubble.leader.setLeader(std::min(bubble.leader.getLeader(), otherBubble.leader.getLeader()));

                    if (overlaps(pos, Colliders::bubbleRepelCollider, otherPos, Colliders::bubbleRepelCollider)) {
                        int dx = pos.x - otherPos.x;
                        int dy = pos.y - otherPos.y;
                        bubbleRepelDirection = bubbleRepelDirection.Add(dx, dy);
                    }
                }
            }

            bubbleRepelDirection.IntegerNormalize();
        }

        // PRINT_INFO("Bubble {} has leader {}", (int)entity, bubble.leader.getLeader());

        const int BUBBLE_REPEL_SPEED = 2;
        Vector2Int velocity = airflowVelocity.Add(BUBBLE_REPEL_SPEED * bubbleRepelDirection.X, BUBBLE_REPEL_SPEED * bubbleRepelDirection.Y);
        velocity = velocity.Add(dragonBubblePushVelocity, 0);

        if (velocity.X == 0 && Random::Get().Chance(0.05f)) {
            velocity.X = 2 * Random::Get().GetDirection();
        }

        if (bubble.isWaitingForPop()) {
            bubble.popFrame--;
            if (bubble.popFrame == 0 || collidesWithDragonSpikes(registry, pos, col)) {
                // Note: this should only be reached by actual bubbles and not bubbled enemies,
                // so we don't care that much, how we set popComponent.poppedFromLifeTime
                Defer(entity, &MakeBubbleAndGroupPopFromDragonSpikes, PopFromDragonSpikes);
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
                Defer(entity, &MakeBubbleAndGroupPopFromLifetime, PopFromLifeTime);
            }
            if (collidesWithDragonSpikes(registry, pos, col)) {
                Defer(entity, &MakeBubbleAndGroupPopFromDragonSpikes, PopFromDragonSpikes);
            }
        }

        // Debug::DrawCollider(pos.x, pos.y, Colliders::bubbleRepelCollider);
        // Debug::DrawCollider(pos.x, pos.y, Colliders::bubblePopCollider, BLUE);
    }
}
