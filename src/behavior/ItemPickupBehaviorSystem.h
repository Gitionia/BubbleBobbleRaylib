#pragma once

#include "../ecs/SystemBase.h"

class ItemPickupBehaviorSystem : public SystemBase {
    SYSTEM_DEF(ItemPickupBehaviorSystem, SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

  protected:
    void Update() override;

  private:
    const Sound *getRandomItemPickupSound();

  private:
    std::array<const Sound *, 3> itemPickupSounds{};
};
