#pragma once

#include "../ecs/SystemBase.h"

class TrashCanBehaviorSystem : public SystemBase {
    SYSTEM_DEF(TrashCanBehaviorSystem)

  public:
    void Init() override;

  protected:
    void Update() override;
};
