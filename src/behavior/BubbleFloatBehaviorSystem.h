#pragma once

#include "../ecs/SystemBase.h"

class BubbleFloatBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleFloatBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};
