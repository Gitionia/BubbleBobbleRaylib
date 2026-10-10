#include "LeaderboardInputBehaviorSystem.h"

#include "../../gui/GUIFactory.h"
#include "../../network/OnlineLeaderboard.h"

void LeaderboardInputBehaviorSystem::Setup() {

    levelReached = -1;
    score = -1;

    auto levelReachedEvents = eventSystem.ReadEvent(SEND_LEVEL_REACHED_FOR_LEADERBOARD_INPUT);
    if (levelReachedEvents.size()) {
        levelReached = levelReachedEvents.at(0).data;
    }
    auto scoreEvents = eventSystem.ReadEvent(SEND_SCORE_FOR_LEADERBOARD_INPUT);
    if (scoreEvents.size()) {
        score = scoreEvents.at(0).data;
    }

    infoGUIText[0] = GUIFactory::CreateStaticText(20, 20, "Do you want to add your score", WHITE);
    infoGUIText[1] = GUIFactory::CreateStaticText(20, 50, "To the global leaderboard?", WHITE);
    if (levelReached >= 0) {
        infoGUIText[2] = GUIFactory::CreateStaticText(60, 100, std::format("Round: {}", levelReached), WHITE);
    }
    if (score >= 0) {
        infoGUIText[3] = GUIFactory::CreateStaticText(260, 100, "Score: " + std::to_string(score), WHITE);
    }

    noButton = GUIFactory::CreateButtonAutoSize(140, 150, "No", WHITE, BLACK, PINK);
    yesButton = GUIFactory::CreateButtonAutoSize(300, 150, "Yes", WHITE, BLACK, PINK);
    noYesButtonGroup = GUIFactory::CreateButtonGroup({noButton, yesButton}, GUILayout::HORIZONTAL);

    eventSystem.Notify(entt::null, SET_ACTIVE_GUI_BUTTON_GROUP, (int64_t)noYesButtonGroup);

    nameInfoGUIText = GUIFactory::CreateStaticText(120, 220, "Name: ", GRAY);

    nameString = std::make_shared<std::string>("_");
    nameString->reserve(4);
    playerNameGUIText = GUIFactory::CreateDynamicText(150, 250, nameString, GRAY);

    cancelButton = GUIFactory::CreateButtonAutoSize(140, 320, "Cancel", WHITE, BLACK, PINK);
    sendButton = GUIFactory::CreateButtonAutoSize(300, 320, "Send", WHITE, BLACK, PINK);
    cancelSendButtonGroup = GUIFactory::CreateButtonGroup({cancelButton, sendButton}, GUILayout::HORIZONTAL);
}

void LeaderboardInputBehaviorSystem::Update() {
    if (state == NoYes) {
        if (GUI::IsButtonPressed(noButton)) {
            eventSystem.Notify(entt::null, EXIT_LEADERBOARD_INPUT_SCREEN, 0);

        } else if (GUI::IsButtonPressed(yesButton)) {
            registry.get<GUIStaticText>(nameInfoGUIText).TextColor = PINK;
            registry.get<GUIDynamicText>(playerNameGUIText).TextColor = PINK;
            eventSystem.Notify(entt::null, UNFOCUS_GUI_BUTTON_GROUP, 0);

            state = EnterName;
        }
    } else if (state == EnterName) {
        if (Input::GetYAxis() > 0 || IsKeyPressed(KEY_BACKSPACE) && nameString->size() == 1) {
            state = NoYes;
            registry.get<GUIStaticText>(nameInfoGUIText).TextColor = GRAY;
            registry.get<GUIDynamicText>(playerNameGUIText).TextColor = GRAY;
            eventSystem.Notify(entt::null, SET_ACTIVE_GUI_BUTTON_GROUP, (int64_t)noYesButtonGroup);
            eventSystem.Notify(entt::null, SET_FOCUSED_GUI_BUTTON, 1);
        }

        char c = GetCharPressed();
        if (std::islower(c) || std::isupper(c) || std::isdigit(c)) {
            if (nameString->at(nameString->size() - 1) == '_') {
                nameString->at(nameString->size() - 1) = std::toupper(c);
                if (nameString->size() <= 2) {
                    *nameString.get() += '_';
                }
            }

        } else if (IsKeyPressed(KEY_BACKSPACE)) {
            if (nameString->size() == 0)
                nameString->resize(1, '_');

            if (nameString->size() >= 2 && nameString->at(nameString->size() - 1) == '_') {
                nameString->pop_back();
                nameString->at(nameString->size() - 1) = '_';
            } else {
                nameString->at(nameString->size() - 1) = '_';
            }
        }
        if (nameString->size() == 3 && nameString->at(2) != '_') {
            state = CancelSend;
            eventSystem.Notify(entt::null, SET_ACTIVE_GUI_BUTTON_GROUP, (int64_t)cancelSendButtonGroup);
        }
    } else if (state == CancelSend) {
        if (GUI::IsButtonPressed(cancelButton)) {
            PRINT_INFO("Pressed cancel");
            state = NoYes;
            registry.get<GUIStaticText>(nameInfoGUIText).TextColor = GRAY;
            registry.get<GUIDynamicText>(playerNameGUIText).TextColor = GRAY;
            nameString->resize(0, ' ');
            nameString->resize(1, '_');

            eventSystem.Notify(entt::null, SET_ACTIVE_GUI_BUTTON_GROUP, (int64_t)noYesButtonGroup);
            eventSystem.Notify(entt::null, SET_FOCUSED_GUI_BUTTON, 1);

        } else if (GUI::IsButtonPressed(sendButton)) {

            if (score >= 0 && levelReached >= 0) {
                SendScoreToLeaderboard(*nameString, score, levelReached);
            }
            eventSystem.Notify(entt::null, EXIT_LEADERBOARD_INPUT_SCREEN, 0);
        }
    }
}

void LeaderboardInputBehaviorSystem::CleanUp() {
    for (auto guiText : infoGUIText) {
        registry.destroy(guiText);
    }
    registry.destroy(noButton);
    registry.destroy(yesButton);
    registry.destroy(noYesButtonGroup);

    registry.destroy(nameInfoGUIText);
    registry.destroy(playerNameGUIText);

    registry.destroy(cancelButton);
    registry.destroy(sendButton);
    registry.destroy(cancelSendButtonGroup);
}