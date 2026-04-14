#include "VictorySceneBehaviorSystem.h"

#include "../app/WindowConfig.h"

void VictorySceneBehaviorSystem::Init() {
}

void VictorySceneBehaviorSystem::Setup() {
    int width = 16 * 2;
    Vector2Int center{TARGET_WINDOW_WIDTH / 4 - width / 2, TARGET_WINDOW_HEIGHT / 4};
    humanLeft = EntityFactory::CreateSimpleSprite(center.Add(-width / 2, 0), -1, GetSpriteHandle("HumanPlayer-Green-1"), WHITE, {2, 2});
    humanRight = EntityFactory::CreateSimpleSprite(center.Add(width / 2, 0), -1, GetSpriteHandle("HumanPlayer-Blue-1"), WHITE, {2, 2});

    std::array<const char *, 4> texts = {" NOW, IT IS THE BEGINNING OF A  \n",
                                         " FANTASTIC STORY!! LET'S MAKE A \n",
                                         "JOURNEY TO THE CAVE OF MONSTERS!\n",
                                         "            GOOD LUCK!          "};

    for (int i = 0; i < texts.size(); i++) {
        int yOffset = i * 64;
        textEntities.at(i) = EntityFactory::CreateConstantUIText(center.Add(-160, -80 + yOffset), texts.at(i), RED, 32, -4);
    }
}

void VictorySceneBehaviorSystem::Update() {
    if (eventSystem.ReadEvent(DELETE_VICTORY_SCENE).size()) {
        registry.destroy(humanLeft);
        registry.destroy(humanRight);
        registry.destroy(textEntities.begin(), textEntities.end());

        DBG_CHECK(Debug::GetNumberOfEntities() == 0, std::format("There should be no entities after deleting the intro scene, but there are {}!", Debug::GetNumberOfEntities()));
    }
}