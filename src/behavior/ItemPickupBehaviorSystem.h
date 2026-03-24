#pragma once

#include "../ecs/SystemBase.h"

class ItemPickupBehaviorSystem : public SystemBase {
    SYSTEM_DEF(ItemPickupBehaviorSystem)

  protected:
    void Update() override;
};
