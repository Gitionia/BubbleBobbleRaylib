#pragma once

#include "../ecs/SystemBase.h"

class BubblePopBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubblePopBehaviorSystem)

  public:
    void Update() override;
};
