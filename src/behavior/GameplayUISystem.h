#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"

class GameplayUISystem : public SystemBase {
    SYSTEM_DEF(GameplayUISystem, SystemTypeFlags::GAMEPLAY | SystemTypeFlags::GAME_UI)

  protected:
    void Init() override;
    void Update() override;

  private:
    int points = 0;
    entt::entity scoreEntity = entt::null;

    int level = 1;
    entt::entity levelCounterInCorner = entt::null;
};
