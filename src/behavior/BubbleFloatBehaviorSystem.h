#pragma once

#include "../ecs/SystemBase.h"

class BubbleFloatBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleFloatBehaviorSystem)

  public:
    void Update() override;
};
