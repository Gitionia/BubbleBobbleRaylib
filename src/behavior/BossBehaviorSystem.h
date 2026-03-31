#pragma once

#include "../ecs/SystemBase.h"

class BossBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BossBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};