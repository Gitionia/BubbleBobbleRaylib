#pragma once

#include "../ecs/SystemBase.h"

#include "../graphics/Animations.h"

class DragonBehaviorSystem : public SystemBase {
    SYSTEM_DEF(DragonBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    const Sound *shootSound = 0;
    const Sound *jumpSound = 0;
    Animation idleAnimation{};
};
