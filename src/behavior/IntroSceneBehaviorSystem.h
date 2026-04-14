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
    std::vector<entt::entity> bubbles;
    entt::entity textEntity;
  };
