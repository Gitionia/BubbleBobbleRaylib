#include "IntroSceneBehaviorSystem.h"
#include "entt/entity/fwd.hpp"

#include "../app/WindowConfig.h"
#include <cmath>

void IntroSceneBehaviorSystem::Init() {
}

void IntroSceneBehaviorSystem::Setup() {
    int dragonWidth = 32 * 2;
    Vector2Int center{TARGET_WINDOW_WIDTH / 4 - dragonWidth / 2, TARGET_WINDOW_HEIGHT / 4};

    int xOffset = 140;
    int yOffset = -10;

    greenDragonCenterPos = center.Add(-xOffset, yOffset);
    blueDragonCenterPos = center.Add(xOffset, yOffset);

    greenDragon = EntityFactory::CreateSimpleAnimatedSprite(greenDragonCenterPos, -1,
                                                            GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_GREEN)), {2, 2});
    blueDragon = EntityFactory::CreateSimpleAnimatedSprite(blueDragonCenterPos, -1,
                                                           GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_BLUE)), {2, 2});

    std::array<const char *, 4> texts = {" NOW, IT IS THE BEGINNING OF A  \n",
                                         " FANTASTIC STORY!! LET'S MAKE A \n",
                                         "JOURNEY TO THE CAVE OF MONSTERS!\n",
                                         "            GOOD LUCK!          "};

    for (int i = 0; i < texts.size(); i++) {
        int yOffset = i * 32;
        textEntities.at(i) = EntityFactory::CreateConstantUIText({32, 32 + yOffset}, texts.at(i), RED, 32, -4);
    }

    counter = 0;
}

void IntroSceneBehaviorSystem::Update() {
    if (eventSystem.ReadEvent(DELETE_INTRO_SCENE).size()) {
        registry.destroy(greenDragon);
        registry.destroy(blueDragon);
        registry.destroy(textEntities.begin(), textEntities.end());

        auto bubbles = registry.view<PositionAnimationComponent>();
        registry.destroy(bubbles.begin(), bubbles.end());

        DBG_CHECK(Debug::GetNumberOfEntities() == 0, std::format("There should be no entities after deleting the intro scene, but there are {}!", Debug::GetNumberOfEntities()));

    } else if (counter < COUNTER_FINISHED) {
        counter++;

        if (counter % 2 == 0) {
            Vector2Int bubbleStart{TARGET_WINDOW_WIDTH / 4 - 14 / 2, TARGET_WINDOW_HEIGHT / 4};
            int radius = 400;
            float angle = counter / 0.818f + PI / 6;
            int circleX = std::cos(angle) * radius;
            int circleY = std::sin(angle) * radius;
            entt::entity newBubble = EntityFactory::CreateSimpleSprite(bubbleStart, -1, GetSpriteHandle("WhiteBubble-Idle-1"), WHITE, {1, 1});
            registry.emplace<PositionAnimationComponent>(newBubble, bubbleStart, bubbleStart.Add(circleX, circleY), 80, PositionAnimationComponent::DELETE_ENTITY);
        }

        entt::entity dragons[2] = {greenDragon, blueDragon};
        Vector2Int dragonCenterPositions[2] = {greenDragonCenterPos, blueDragonCenterPos};
        for (int i = 0; i < 2; i++) {
            auto dragon = dragons[i];
            auto dragonCenterPos = dragonCenterPositions[i];

            auto &animatedSprite = registry.get<SimpleAnimatedSpriteComponent>(dragon);
            RenderData &renderData = registry.get<RenderData>(dragon);
            Position &pos = registry.get<Position>(dragon);

            animatedSprite.animator.Update();
            if (animatedSprite.animator.IsFinished()) {
                animatedSprite.animator.Reset();
            }
            renderData.spriteHandle = animatedSprite.animator.GetSpriteHandle();

            int radiusX = 10;
            int radiusY = 16;
            int signX = i == 0 ? 1 : -1;
            float angle = counter / 12.0f;
            int circleX = std::cos(angle) * radiusX * signX;
            int circleY = std::sin(angle) * radiusY;

            pos.x = dragonCenterPos.X + circleX;
            pos.y = dragonCenterPos.Y + circleY;
        }

    } else {
        eventSystem.Notify((entt::entity)0, INTRO_SCENE_FINISHED, 0);
    }
}