#include "BossBehaviorSystem.h"

#include "../level/Physics.h"

void BossBehaviorSystem::Update() {
    auto view = registry.view<Position, BossComponent, RenderData>(entt::exclude<BubbleFloatComponent, BubblePopComponent>);
    for (auto entity : view) {
        auto [pos, boss, renderData] = view.get(entity);

        const Collider &collider = Colliders::bossCollider;

        boss.animator.Update();
        if (boss.animator.IsFinished()) {
            boss.animator.Reset();
        }
        renderData.spriteHandle = boss.animator.GetSpriteHandle();

        const int BOSS_WIDTH = BP_SIZE(8, 0);
        const int BOSS_HEIGHT = BP_SIZE(8, 0);

        switch (boss.state) {

        case EnemyAnimationType::NORMAL: {

            std::optional<entt::entity> bubble = getCollidingShootingBubble(registry, pos, collider);
            if (bubble.has_value()) {
                boss.state = EnemyAnimationType::BUBBLED;
                boss.animator.SetNewAnimation(&GetAnimation(GetEnemyAnimationName(EnemyType::BOSS, EnemyAnimationType::BUBBLED)));
                boss.timeTillCanBubbledCanBePopped = 90;
                continue;
            }

            int speed = BP_SIZE(0, 4);
            pos.x += speed * -boss.xDir;
            if (pos.x < BP_SIZE(2, 0)) {
                pos.x = BP_SIZE(2, 0);
                boss.xDir *= -1;

            } else if (pos.x + BOSS_WIDTH >= BP_SIZE(LevelTilemap::WIDTH + 2, 0)) {
                pos.x = BP_SIZE(LevelTilemap::WIDTH + 2, 0) - BOSS_WIDTH;
                boss.xDir *= -1;
            }

            pos.y += speed * boss.yDir;
            if (pos.y < BP_SIZE(1, 0)) {
                pos.y = BP_SIZE(1, 0);
                boss.yDir *= -1;

            } else if (pos.y + BOSS_HEIGHT >= BP_SIZE(LevelTilemap::HEIGHT - 1, 0)) {
                pos.y = BP_SIZE(LevelTilemap::HEIGHT - 1, 0) - BOSS_HEIGHT;
                boss.yDir *= -1;
            }
            break;
        }
        case EnemyAnimationType::SHOOTING:
        case EnemyAnimationType::BUBBLED:
            if (boss.timeTillCanBubbledCanBePopped > 0) {
                boss.timeTillCanBubbledCanBePopped--;
            }
            if (boss.timeTillCanBubbledCanBePopped <= 0 && collidesWithDragonSpikes(registry, pos, collider)) {
                boss.state = EnemyAnimationType::ITEM;
                boss.animator.SetNewAnimation(&GetAnimation(GetEnemyAnimationName(EnemyType::BOSS, EnemyAnimationType::ITEM)));

                if (pos.x + BOSS_WIDTH / 2 < BP_SIZE(LevelTilemap::WIDTH / 2 + 2, 0)) {
                    boss.xDir = 1;
                } else {
                    boss.xDir = -1;
                }
                continue;
            }
            break;

        case EnemyAnimationType::ITEM:
            const int SPEED = BP_SIZE(0, 4);
            pos.x += SPEED * boss.xDir;
            pos.y -= SPEED;
            break;
        }
    }
}
