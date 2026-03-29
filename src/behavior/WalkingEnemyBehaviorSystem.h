#pragma once

#include "../ecs/SystemBase.h"

class WalkingEnemyBehaviorSystem : public SystemBase {
    SYSTEM_DEF(WalkingEnemyBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;
};
