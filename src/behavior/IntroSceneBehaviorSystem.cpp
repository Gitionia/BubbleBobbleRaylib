#include "IntroSceneBehaviorSystem.h"

void IntroSceneBehaviorSystem::Init() {
    bubbles.reserve(200);
}

void IntroSceneBehaviorSystem::Setup() {
    greenDragon = EntityFactory::CreateSimpleAnimatedSprite({100, 200}, -1,
                                                            GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_GREEN)));
    greenDragon = EntityFactory::CreateSimpleAnimatedSprite({400, 200}, -1,
                                                            GetAnimation(GetDragonAnimation(DragonAnimationType::BIG_BUBBLE_IDLE, DRAGON_BLUE)));
}

void IntroSceneBehaviorSystem::Update() {
}