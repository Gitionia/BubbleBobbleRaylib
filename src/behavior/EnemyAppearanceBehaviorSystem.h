#pragma once

#include "../ecs/SystemBase.h"

class EnemyAppearanceBehaviorSystem : public SystemBase {
    SYSTEM_DEF(EnemyAppearanceBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};
