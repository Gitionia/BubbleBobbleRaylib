#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entity/fwd.hpp"
#include <vector>

class IntroSceneBehaviorSystem : public SystemBase {
    SYSTEM_DEF(IntroSceneBehaviorSystem, SystemTypeFlags::INTRO_SCENE)
  public:
    void Init() override;
    void Setup() override;

  protected:
    void Update() override;

  private:
    entt::entity greenDragon;
    entt::entity blueDragon;
    entt::entity textEntity;
    int counter = 0;

    // Time when intro music stops
    constexpr static int COUNTER_FINISHED = 350;
  };
