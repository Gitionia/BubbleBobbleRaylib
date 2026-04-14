#include "VictorySceneBehaviorSystem.h"

#include "../app/WindowConfig.h"
#include "raylib.h"

void VictorySceneBehaviorSystem::Init() {
}

void VictorySceneBehaviorSystem::Setup() {
    {
        int width = 16 * 2;
        Vector2Int centerSprites{TARGET_WINDOW_WIDTH / 4 - width / 2, TARGET_WINDOW_HEIGHT / 4};
        humanLeft = EntityFactory::CreateSimpleSprite(centerSprites.Add(-width / 2, 90 + 60), -1, GetSpriteHandle("HumanPlayer-Green-1"), WHITE, {2, 2});
        humanRight = EntityFactory::CreateSimpleSprite(centerSprites.Add(width / 2, 90 + 60), -1, GetSpriteHandle("HumanPlayer-Blue-1"), WHITE, {2, 2});
        heart = EntityFactory::CreateSimpleSprite(centerSprites.Add(-width, 30), -1, GetSpriteHandle("BigHeart"), WHITE, {1.5f, 1.5});
    }

    {
        std::array<const char *, 5> gameTexts = {"CONGRATULATIONS",
                                                 " YOU HAVE FINISHED BUBBLE BOBBLE ",
                                                 "AND YOU HAVE FOUND THE TRUE MAGIC",
                                                 "     OF LIFE LOVE FRIENDSHIP     ",
                                                 "          AND HAPPINESS          "};

        gameTextEntities.at(0) = EntityFactory::CreateConstantUIText({128 - 39, 128 - 90}, gameTexts.at(0), YELLOW, 48, -4);

        for (int i = 1; i < gameTexts.size(); i++) {
            int yOffset = i * 32;
            gameTextEntities.at(i) = EntityFactory::CreateConstantUIText({128 - 104, 128 - 65 + yOffset}, gameTexts.at(i), PURPLE, 32, -4);
        }
    }
    {
        std::array<const char *, 34> creditTexts = {"A fan remake ",
                                                    "",
                                                    "Developed by",
                                                    "  Gitionia ",
                                                    "  using raylib",
                                                    "",
                                                    "",
                                                    "",
                                                    "Sprites from spriters-resource",
                                                    "  Random Talking Bush",
                                                    "  Jermungandr",
                                                    "",
                                                    "",
                                                    "",
                                                    "Sounds from archive.org",
                                                    "  Tim Follin",
                                                    "  and others"
                                                    "",
                                                    "",
                                                    "",
                                                    "Sounds from spriters-resource",
                                                    "  John2k4",
                                                    "  J-Sinn"
                                                    "",
                                                    "",
                                                    "",
                                                    "Thanks to Taito",
                                                    "  for making Bubble Bobble",
                                                    "",
                                                    "",
                                                    "",
                                                    "",
                                                    "",
                                                    "",
                                                    "Thank you for playing!",
                                                    "",
                                                };

        int creditOffsetY = 520;
        for (int i = 0; i < creditTextEntities.size(); i++) {
            int yOffset = i * 32;
            creditTextEntities.at(i) = EntityFactory::CreateConstantUIText({32, 128 - 65 + yOffset + creditOffsetY}, creditTexts.at(i), WHITE, 32, -4);
        }
    }
}

void VictorySceneBehaviorSystem::Update() {
    if (eventSystem.ReadEvent(DELETE_VICTORY_SCENE).size()) {
        registry.destroy(humanLeft);
        registry.destroy(humanRight);
        registry.destroy(heart);
        registry.destroy(gameTextEntities.begin(), gameTextEntities.end());
        registry.destroy(creditTextEntities.begin(), creditTextEntities.end());

        DBG_CHECK(Debug::GetNumberOfEntities() == 0, std::format("There should be no entities after deleting the victory scene, but there are {}!", Debug::GetNumberOfEntities()));

        PRINT_INFO("{}", counter);
    } else {
        counter++;
        if (counter > COUNTER_TILL_ENTITIES_MOVE && counter < COUNTER_WHEN_ENTITIES_STOP && counter % 2 == 0) {
            auto view = registry.view<Position>();

            for (auto entity : view) {
                Position &pos = view->get(entity);

                pos.y--;
            }
        }
    }
}