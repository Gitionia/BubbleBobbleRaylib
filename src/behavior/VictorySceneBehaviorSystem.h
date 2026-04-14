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
    entt::entity heart;
    std::array<entt::entity, 5> gameTextEntities;
    std::array<entt::entity, 34> creditTextEntities;

    int counter = 0;
    constexpr static int COUNTER_TILL_ENTITIES_MOVE = 60 * 10;
    constexpr static int COUNTER_WHEN_ENTITIES_STOP = 3400;
};
