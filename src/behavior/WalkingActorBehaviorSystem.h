#pragma once

#include "../ecs/SystemBase.h"
#include "entt/entt.hpp"

#include "../graphics/Animations.h"

class WalkingActorBehaviorSystem : public SystemBase {
    SYSTEM_DEF(WalkingActorBehaviorSystem)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    Animation idleAnimation{};
};
