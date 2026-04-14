#include "VictorySceneBehaviorSystem.h"

#include "../app/WindowConfig.h"
#include "raylib.h"

void VictorySceneBehaviorSystem::Init() {
}

void VictorySceneBehaviorSystem::Setup() {
    int width = 16 * 2;
    Vector2Int center{TARGET_WINDOW_WIDTH / 4 - width / 2, TARGET_WINDOW_HEIGHT / 4};
    humanLeft = EntityFactory::CreateSimpleSprite(center.Add(-width / 2, 90 + 30), -1, GetSpriteHandle("HumanPlayer-Green-1"), WHITE, {2, 2});
    humanRight = EntityFactory::CreateSimpleSprite(center.Add(width / 2, 90 + 30), -1, GetSpriteHandle("HumanPlayer-Blue-1"), WHITE, {2, 2});
    heart = EntityFactory::CreateSimpleSprite(center.Add(-width, 0), -1, GetSpriteHandle("BigHeart"), WHITE, {1.5f, 1.5});

    std::array<const char *, 5> texts = {"CONGRATULATIONS",
                                         " YOU HAVE FINISHED BUBBLE BOBBLE \n",
                                         "AND YOU HAVE FOUND THE TRUE MAGIC\n",
                                         "     OF LIFE LOVE FRIENDSHIP     \n",
                                         "          AND HAPPINESS          "};

    textEntities.at(0) = EntityFactory::CreateConstantUIText(center.Add(-70, -100 - 80), texts.at(0), YELLOW, 48, -4);

    for (int i = 1; i < texts.size(); i++) {
        int yOffset = i * 64;
        textEntities.at(i) = EntityFactory::CreateConstantUIText(center.Add(-200, -130 + yOffset), texts.at(i), PURPLE, 32, -4);
    }
}

void VictorySceneBehaviorSystem::Update() {
    if (eventSystem.ReadEvent(DELETE_VICTORY_SCENE).size()) {
        registry.destroy(humanLeft);
        registry.destroy(humanRight);
        registry.destroy(heart);
        registry.destroy(textEntities.begin(), textEntities.end());

        DBG_CHECK(Debug::GetNumberOfEntities() == 0, std::format("There should be no entities after deleting the victory scene, but there are {}!", Debug::GetNumberOfEntities()));
    }
}