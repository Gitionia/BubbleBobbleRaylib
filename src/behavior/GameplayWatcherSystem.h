#pragma once

#include "../ecs/SystemBase.h"

class GameplayWatcherSystem : public SystemBase {
    SYSTEM_DEF(GameplayWatcherSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};
