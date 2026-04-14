#include "IntroSceneBehaviorSystem.h"

void IntroSceneBehaviorSystem::Init() {
    bubbles.reserve(200);
}

void IntroSceneBehaviorSystem::Setup() {
    greenDragon = EntityFactory::CreateSimpleAnimatedSprite({100, 200}, -1,
                                                            GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_GREEN)), {2, 2});
    blueDragon = EntityFactory::CreateSimpleAnimatedSprite({400, 200}, -1,
                                                           GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_BLUE)), {2, 2});
}

void IntroSceneBehaviorSystem::Update() {
    if (eventSystem.ReadEvent(DELETE_INTRO_SCENE).size()) {
        registry.destroy(greenDragon);
        registry.destroy(blueDragon);

        DBG_CHECK(Debug::GetNumberOfEntities() == 0, std::format("There should be no entities after deleting the intro scene, but there are {}!", Debug::GetNumberOfEntities()));
    }
}