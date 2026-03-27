#pragma once

#include "../ecs/SystemBase.h"

class GameplayUISystem : public SystemBase {
    SYSTEM_DEF(GameplayUISystem, SystemTypeFlags::GAMEPLAY | SystemTypeFlags::GAME_UI)

  protected:
    void Init() override;
    void Update() override;

  private:
    int points = 0; 
    entt::entity textEntity;
    ModifiableUITextComponent *ui;
};
