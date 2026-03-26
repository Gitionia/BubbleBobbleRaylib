#pragma once

#include "../ecs/SystemBase.h"

class PositionAnimationBehaviorSystem : public SystemBase {
    SYSTEM_DEF(PositionAnimationBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};
