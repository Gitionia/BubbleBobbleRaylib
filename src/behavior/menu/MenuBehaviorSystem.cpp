#include "MenuBehaviorSystem.h"

#include "../../gui/GUIFactory.h"

void MenuBehaviorSystem::Setup() {
    button1Player = GUIFactory::CreateButtonAutoSize(150, 150, "1 Player Start", PINK, BLACK, PINK);
    button2Players = GUIFactory::CreateButtonAutoSize(150, 200, "2 Player Start", PINK, BLACK, PINK);
    buttonGroup = GUIFactory::CreateButtonGroup({button1Player, button2Players}, GUILayout::VERTICAL);

    eventSystem.Notify(entt::null, SET_GUI_FOCUS_TO_ENTITY, (int64_t)buttonGroup);
}

void MenuBehaviorSystem::Update() {
    if (GUI::IsButtonPressed(button1Player)) {
        eventSystem.Notify((entt::entity)0, MENU_BUTTON_SINGLEPLAYER_START_WAS_PRESSED, 0);
    } else if (GUI::IsButtonPressed((button2Players))) {
        eventSystem.Notify((entt::entity)0, MENU_BUTTON_MULTIPLAYER_START_WAS_PRESSED, 0);
    }
}

void MenuBehaviorSystem::CleanUp() {
    registry.destroy(buttonGroup);
    registry.destroy(button1Player);
    registry.destroy(button2Players);
}
