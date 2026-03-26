#pragma once

#include "../ecs/SystemBase.h"

class EnemyTumbleBehaviorSystem : public SystemBase {
    SYSTEM_DEF(EnemyTumbleBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;
};
