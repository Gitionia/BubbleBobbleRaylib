#pragma once


#include "../ecs/SystemBase.h"

class EnemyProjectileBehaviorSystem : public SystemBase {
    SYSTEM_DEF(EnemyProjectileBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;
};