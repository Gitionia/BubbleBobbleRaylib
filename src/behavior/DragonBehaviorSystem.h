#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entt.hpp"

#include "../graphics/Animations.h"

class DragonBehaviorSystem : public SystemBase {
    SYSTEM_DEF(DragonBehaviorSystem)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    Animation idleAnimation{};
};
