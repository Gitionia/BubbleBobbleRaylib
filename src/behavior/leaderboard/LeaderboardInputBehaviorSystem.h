#pragma once

#include "../../ecs/SystemBase.h"
#include "../../network/OnlineLeaderboard.h"

class LeaderboardInputBehaviorSystem : public SystemBase {
    SYSTEM_DEF(LeaderboardInputBehaviorSystem, 0)
  public:
    void Setup() override;
    void CleanUp() override;

  protected:
    void Update() override;

  private:
    enum State {
        NoYes = 0,
        EnterName = 1,
        CancelOrSend = 2,
        SendWaiting = 3,
        DisplaySendStatusButCantExitYet = 4,
        DisplaySendStatusCanExit = 5,
    };

    int score = -1;
    int levelReached;
    int timerTillCanExit = 120;
    int sendTimeOut = 60;

    State state = NoYes;
    std::shared_ptr<std::string> nameString;
    std::shared_ptr<HttpRequestResult> httpResult = nullptr;

    std::array<entt::entity, 4> infoGUIText;


    entt::entity noButton;
    entt::entity yesButton;
    entt::entity noYesButtonGroup;

    entt::entity nameInfoGUIText;
    entt::entity playerNameGUIText;

    entt::entity cancelButton;
    entt::entity sendButton;
    entt::entity cancelSendButtonGroup;

    entt::entity successGUIText;
    entt::entity notSuccessGUIText;
    entt::entity endLeaderboardGUIText;
};
