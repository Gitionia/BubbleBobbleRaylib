#pragma once

#include "../ecs/SystemBase.h"

class EnemyItemDiceBehaviorSystem : public SystemBase {
    SYSTEM_DEF(EnemyItemDiceBehaviorSystem)

  public:
    void Init() override;

  protected:
    void Update() override;
};
