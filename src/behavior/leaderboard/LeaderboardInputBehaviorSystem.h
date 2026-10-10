#pragma once

#include "../../ecs/SystemBase.h"
#include <memory>

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
        CancelSend = 2
    };

    int score = -1;
    int levelReached;

    State state = NoYes;
    std::shared_ptr<std::string> nameString;

    std::array<entt::entity, 4> infoGUIText;

    entt::entity noButton;
    entt::entity yesButton;
    entt::entity noYesButtonGroup;

    entt::entity nameInfoGUIText;
    entt::entity playerNameGUIText;

    entt::entity cancelButton;
    entt::entity sendButton;
    entt::entity cancelSendButtonGroup;
};
