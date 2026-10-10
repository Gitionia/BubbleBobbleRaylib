#pragma once

#include "../ecs/SystemBase.h"

class BubblePopBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubblePopBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
  };
