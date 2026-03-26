#pragma once

#include "../ecs/SystemBase.h"

class ItemPickupBehaviorSystem : public SystemBase {
    SYSTEM_DEF(ItemPickupBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};
