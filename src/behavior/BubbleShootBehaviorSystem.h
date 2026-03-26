#pragma once

#include "../ecs/SystemBase.h"

class BubbleShootBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleShootBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};
