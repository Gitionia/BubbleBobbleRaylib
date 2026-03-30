#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entity/fwd.hpp"

class BubblePopBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubblePopBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
  };
