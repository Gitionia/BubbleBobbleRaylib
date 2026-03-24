#pragma once

#include "../ecs/SystemBase.h"

class PositionAnimationBehaviorSystem : public SystemBase {
    SYSTEM_DEF(PositionAnimationBehaviorSystem)

  protected:
    void Update() override;
};
