#include "PositionAnimationBehaviorSystem.h"

void PositionAnimationBehaviorSystem::Update() {
    auto view = registry.view<Position, PositionAnimationComponent>();

    for (auto entity : view) {
        auto [pos, animation] = view.get(entity);

        if (animation.progress == animation.totalFrameCount) {
            pos.x = animation.end.X;
            pos.y = animation.end.Y;

            switch (animation.action) {
            case PositionAnimationComponent::DELETE_ENTITY:
                Destroy(entity);
                break;
            }

        } else {

            float progress = (float)animation.progress / (float)animation.totalFrameCount;
            int x = animation.start.X + (int)((animation.end.X - animation.start.X) * progress);
            int y = animation.start.Y + (int)((animation.end.Y - animation.start.Y) * progress);

            pos.x = x;
            pos.y = y;

            animation.progress++;
        }
    }
}
