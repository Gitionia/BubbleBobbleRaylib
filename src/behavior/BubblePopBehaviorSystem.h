#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entity/fwd.hpp"

class BubblePopBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubblePopBehaviorSystem)

  public:
    void Init() override;
  protected:
    void Update() override;

  private:
    void popAdjacentBubbles(const Position &position, const BubblePopComponent& popComp);
  };
