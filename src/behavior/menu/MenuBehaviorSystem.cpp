#include "MenuBehaviorSystem.h"

#include "../../gui/GUIFactory.h"

void MenuBehaviorSystem::Setup() {
    int xButtonLeft = 50;

    button1Player = GUIFactory::CreateButtonAutoSize(xButtonLeft, 100, "1 Player Start", PINK, BLACK, PINK);
    button2Players = GUIFactory::CreateButtonAutoSize(xButtonLeft, 150, "2 Player Start", PINK, BLACK, PINK);

    buttonOpenLeaderboard = GUIFactory::CreateButtonAutoSize(xButtonLeft, 200, "Open Online Leaderboard", PINK, BLACK, PINK);
    leaderboardGUIText = GUIFactory::CreateStaticText(xButtonLeft, 230, "Opens url in browser", WHITE, 16, 0);
    urlGUIText = GUIFactory::CreateStaticText(xButtonLeft, 246, "https://gitionia.github.io/bubble-bobble-raylib-online/", WHITE, 16, 0);

// Hide quit button on web version, because you can't easily quit emscripten
#ifdef PLATFORM_DESKTOP
    buttonQuit = GUIFactory::CreateButtonAutoSize(xButtonLeft, 320, "Quit", PINK, BLACK, PINK);
    buttonGroup = GUIFactory::CreateButtonGroup({button1Player, button2Players, buttonOpenLeaderboard, buttonQuit}, GUILayout::VERTICAL);
#else
    buttonGroup = GUIFactory::CreateButtonGroup({button1Player, button2Players, buttonOpenLeaderboard}, GUILayout::VERTICAL);
#endif

    eventSystem.Notify(entt::null, SET_ACTIVE_GUI_BUTTON_GROUP, (int64_t)buttonGroup);
}

void MenuBehaviorSystem::Update() {
    if (GUI::IsButtonPressed(button1Player)) {
        eventSystem.Notify((entt::entity)0, MENU_BUTTON_SINGLEPLAYER_START_WAS_PRESSED, 0);
    } else if (GUI::IsButtonPressed((button2Players))) {
        eventSystem.Notify((entt::entity)0, MENU_BUTTON_MULTIPLAYER_START_WAS_PRESSED, 0);
    }else if (GUI::IsButtonPressed((buttonOpenLeaderboard))) {
        OpenURL("https://gitionia.github.io/bubble-bobble-raylib-online/");

#ifdef PLATFORM_DESKTOP
    } else if (GUI::IsButtonPressed((buttonQuit))) {
        eventSystem.Notify((entt::entity)0, REQUEST_APPLICATION_QUIT, 0);
#endif
    }
}

void MenuBehaviorSystem::CleanUp() {
    registry.destroy(buttonGroup);
    registry.destroy(button1Player);
    registry.destroy(button2Players);
    registry.destroy(buttonOpenLeaderboard);
    registry.destroy(leaderboardGUIText);
    registry.destroy(urlGUIText);

#ifdef PLATFORM_DESKTOP
    registry.destroy(buttonQuit);
#endif
}
