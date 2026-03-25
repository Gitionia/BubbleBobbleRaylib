#pragma once

#include "../ecs/SystemBase.h"

class GameplayWatcherSystem : public SystemBase {
    SYSTEM_DEF(GameplayWatcherSystem)

  protected:
    void Update() override;
};
