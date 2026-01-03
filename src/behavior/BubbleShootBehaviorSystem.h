#pragma once

#include "../ecs/SystemBase.h"

class BubbleShootBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleShootBehaviorSystem)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    void makeBubbleFloating(entt::entity entity);

  private:
    std::vector<entt::entity> entitiesToMakeFloating;
};
