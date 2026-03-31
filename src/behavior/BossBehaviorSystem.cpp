#include "BossBehaviorSystem.h"

void BossBehaviorSystem::Update() {
    auto view = registry.view<Position, BossComponent, RenderData>(entt::exclude<BubbleFloatComponent, BubblePopComponent>);
    for (auto entity : view) {
        auto [pos, boss, renderData] = view.get(entity);

        const int BOSS_WIDTH = BP_SIZE(8, 0);
        const int BOSS_HEIGHT = BP_SIZE(8, 0);

        boss.animator.Update();
        if (boss.animator.IsFinished()) {
            boss.animator.Reset();
        }
        renderData.spriteHandle = boss.animator.GetSpriteHandle();

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
    }
}
