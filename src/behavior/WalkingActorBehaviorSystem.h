#pragma once

#include "../ecs/SystemBase.h"

#include "../graphics/Animations.h"

class WalkingActorBehaviorSystem : public SystemBase {
    SYSTEM_DEF(WalkingActorBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    Animation idleAnimation{};
};
