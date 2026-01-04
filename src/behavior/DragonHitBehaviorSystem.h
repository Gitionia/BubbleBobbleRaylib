#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entt.hpp"

#include "../graphics/Animations.h"

class DragonHitBehaviorSystem : public SystemBase {
    SYSTEM_DEF(DragonHitBehaviorSystem)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    Animation idleAnimation{};
};
