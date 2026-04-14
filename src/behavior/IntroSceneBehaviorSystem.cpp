#include "IntroSceneBehaviorSystem.h"
#include "entt/entity/fwd.hpp"

#include "../app/WindowConfig.h"
#include <cmath>

void IntroSceneBehaviorSystem::Init() {
}

void IntroSceneBehaviorSystem::Setup() {
    greenDragon = EntityFactory::CreateSimpleAnimatedSprite({100, 200}, -1,
                                                            GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_GREEN)), {2, 2});
    blueDragon = EntityFactory::CreateSimpleAnimatedSprite({400, 200}, -1,
                                                           GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_BLUE)), {2, 2});

    counter = 0;
}

void IntroSceneBehaviorSystem::Update() {

    if (eventSystem.ReadEvent(DELETE_INTRO_SCENE).size()) {
        registry.destroy(greenDragon);
        registry.destroy(blueDragon);

        auto bubbles = registry.view<PositionAnimationComponent>();
        registry.destroy(bubbles.begin(), bubbles.end());

        DBG_CHECK(Debug::GetNumberOfEntities() == 0, std::format("There should be no entities after deleting the intro scene, but there are {}!", Debug::GetNumberOfEntities()));

    } else {
        counter++;

        Vector2Int start{TARGET_WINDOW_WIDTH / 4, TARGET_WINDOW_HEIGHT / 4};
        
        int radius = 400;
        float angle = counter / 1.7f + PI / 6;
        int circleX = std::cos(angle) * radius;
        int circleY = std::sin(angle) * radius;
        entt::entity newBubble = EntityFactory::CreateSimpleSprite(start, -1, GetSpriteHandle("WhiteBubble-Idle-1"), WHITE, {1, 1});
        registry.emplace<PositionAnimationComponent>(newBubble, start, start.Add(circleX, circleY), 80, PositionAnimationComponent::DELETE_ENTITY);
    }
}