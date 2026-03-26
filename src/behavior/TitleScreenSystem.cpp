#include "TitleScreenSystem.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"

void TitleScreenSystem::Init() {
    EntityFactory::CreateSimpleSprite({80, 0}, -1, GetSpriteHandle("TitleScreen-Title"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({ 0, 288}, 1, GetSpriteHandle("TitleScreen-Dragon"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({ 390, 288}, -1, GetSpriteHandle("TitleScreen-Dragon"), DARKBLUE, {2, 2});
    
    EntityFactory::CreateSimpleSprite({ 56, 122}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({ -5, 250}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({ 418, 38}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({ 486, 78}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});
    EntityFactory::CreateSimpleSprite({ 456, 222}, -1, GetSpriteHandle("Bubble-Green-Idle-1"), WHITE, {2, 2});

    anyKey = EntityFactory::CreateConstantUIText( {324, 688}, "Press any key", WHITE, 32, -4);
    EntityFactory::CreateConstantUIText( {830, 4}, "Made by\nGitionia", WHITE, 24, 0);
}

void TitleScreenSystem::Update() {
    timeCounter++;
    if ((timeCounter / 62) % 2 == 0) {
        registry.get<ConstUITextComponent>(anyKey).color = WHITE;
    } else {
        registry.get<ConstUITextComponent>(anyKey).color = BLACK;
    }
    
}
