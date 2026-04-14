#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entity/fwd.hpp"

class VictorySceneBehaviorSystem : public SystemBase {
    SYSTEM_DEF(VictorySceneBehaviorSystem, SystemTypeFlags::VICTORY_SCENE)
  public:
    void Init() override;
    void Setup() override;

  protected:
    void Update() override;

  private:
    entt::entity humanLeft;
    entt::entity humanRight;
    std::array<entt::entity, 4> textEntities;
};
