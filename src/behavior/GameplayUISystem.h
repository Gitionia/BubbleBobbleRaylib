#pragma once

#include "../ecs/SystemBase.h"

class GameplayUISystem : public SystemBase {
    SYSTEM_DEF(GameplayUISystem, SystemTypeFlags::GAMEPLAY | SystemTypeFlags::GAME_UI)

  protected:
    void Init() override;
    void Update() override;

  public:
    inline static int points = 0;

  private:
    entt::entity scoreEntity = entt::null;

    int level = 1;
    entt::entity levelCounterInCorner = entt::null;
};
