#pragma once

#include "../ecs/SystemBase.h"

class BubbleShootBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleShootBehaviorSystem)

  protected:
    void Update() override;
};
