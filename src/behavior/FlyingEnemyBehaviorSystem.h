#pragma once

#include "../ecs/SystemBase.h"

class FlyingEnemyBehaviorSystem : public SystemBase {
    SYSTEM_DEF(FlyingEnemyBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;
};
