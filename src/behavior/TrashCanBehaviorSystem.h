#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entt.hpp"

class TrashCanBehaviorSystem : public SystemBase {
    SYSTEM_DEF(TrashCanBehaviorSystem)

  public:
    void Init() override;

  protected:
    void Update() override;
};
