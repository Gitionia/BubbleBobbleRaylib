#pragma once

#include "../ecs/SystemBase.h"

class TrashCanBehaviorSystem : public SystemBase {
    SYSTEM_DEF(TrashCanBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;
};
