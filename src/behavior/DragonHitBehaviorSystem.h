#pragma once

#include "../ecs/SystemBase.h"

#include "../graphics/Animations.h"

class DragonHitBehaviorSystem : public SystemBase {
    SYSTEM_DEF(DragonHitBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    Animation idleAnimation{};
};
