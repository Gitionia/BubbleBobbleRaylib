#pragma once

#include "../ecs/SystemBase.h"

class BubbleBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleBehaviorSystem)

  public:
    void Init() override;
    void Update() override;

  private:
    void makeBubbleFloating(entt::entity entity);

  private:
    std::vector<entt::entity> entitiesToMakeFloating;
};
