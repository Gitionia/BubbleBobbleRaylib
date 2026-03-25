#pragma once

#include "../ecs/SystemBase.h"

class BubblePopBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubblePopBehaviorSystem)

  protected:
    void Update() override;

  private:
    void popAdjacentBubbles(const Position &position);
  };
