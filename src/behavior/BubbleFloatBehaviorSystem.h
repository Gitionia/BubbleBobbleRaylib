#pragma once

#include "../ecs/SystemBase.h"

class BubbleFloatBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleFloatBehaviorSystem)

  protected:
    void Update() override;
};
