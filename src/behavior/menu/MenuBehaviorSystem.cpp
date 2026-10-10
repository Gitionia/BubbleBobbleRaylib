#include "MenuBehaviorSystem.h"

#include "../../gui/GUIFactory.h"
#include "entt/entity/fwd.hpp"

void MenuBehaviorSystem::Setup() {
    button1Player = GUIFactory::CreateButton(150, 150, 400, 50, "1 Player Start", PINK);
    button2Players = GUIFactory::CreateButton(150, 200, 400, 50, "2 Player Start", PINK);
    buttonGroup = GUIFactory::CreateButtonGroup({button1Player, button2Players}, GUILayout::VERTICAL);
}

void MenuBehaviorSystem::Update() {
    if (GUI::IsButtonPressed(button1Player)) {
        eventSystem.Notify((entt::entity)0, MENU_BUTTON_SINGLEPLAYER_START_WAS_PRESSED, 0);
    } else if (GUI::IsButtonPressed((button2Players))) {
        eventSystem.Notify((entt::entity)0, MENU_BUTTON_MULTIPLAYER_START_WAS_PRESSED, 0);
    } else {
        if (IsKeyPressed(KEY_SPACE)) {
            eventSystem.Notify((entt::entity)0, MENU_BUTTON_SINGLEPLAYER_START_WAS_PRESSED, 0);
        }
    }
}

void MenuBehaviorSystem::CleanUp() {
    registry.destroy(buttonGroup);
    registry.destroy(button1Player);
    registry.destroy(button2Players);
}
