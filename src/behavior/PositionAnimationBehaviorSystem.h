#pragma once

#include "../ecs/SystemBase.h"

class PositionAnimationBehaviorSystem : public SystemBase {
    SYSTEM_DEF(PositionAnimationBehaviorSystem, SystemTypeFlags::ALL)

  protected:
    void Update() override;
};
