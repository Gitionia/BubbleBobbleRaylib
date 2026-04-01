#include "TitleScreenSystem.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <cstdint>

void TitleScreenSystem::Setup() {
    title = EntityFactory::CreateSimpleSprite({80, 0}, -1, GetSpriteHandle("TitleScreen-Title"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({0, 288}, 1, GetSpriteHandle("TitleScreen-Dragon"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({390, 288}, -1, GetSpriteHandle("TitleScreen-Dragon"), DARKBLUE, {2, 2});

    EntityFactory::CreateSimpleSprite({56, 122}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({-5, 250}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({418, 38}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({486, 78}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({456, 222}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});

    anyKey = EntityFactory::CreateConstantUIText({324, 688}, "Press any key", WHITE, 32, -4);
    EntityFactory::CreateConstantUIText({736, 4}, "Fan Remake\nby Gitionia\nusing raylib", BLUE, 24, 0);
}

void TitleScreenSystem::Update() {

    if (eventSystem.ReadEvent(START_GAMEPLAY).size()) {
        auto sprites = registry.view<SpriteEntityTag>();
        registry.destroy(sprites->begin(), sprites->end());

        auto texts = registry.view<UITextEntityTag>();
        registry.destroy(texts->begin(), texts->end());

    } else {
        timeCounter++;

        int titleCounter = timeCounter % 30;
        Color color;
        int mulG = 2;
        int mulB = 5;
        if ((timeCounter / 30) % 2 == 0) {
            color = {255, (uint8_t)(255 - mulG * titleCounter), (uint8_t)(255 - mulB * titleCounter), 255};
        } else {
            color = {255, (uint8_t)(255 + mulG * (titleCounter - 30)), (uint8_t)(255 + mulB * (titleCounter - 30)), 255};
        }
        registry.get<RenderData>(title).SetColor(color);

        if ((timeCounter / 30) % 2 == 0) {
            registry.get<ConstUITextComponent>(anyKey).color = WHITE;
        } else {
            registry.get<ConstUITextComponent>(anyKey).color = WHITE;
        }
    }
}
