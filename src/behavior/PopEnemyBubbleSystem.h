#pragma once

#include "../ecs/SystemBase.h"

class PopEnemyBubbleSystem: public SystemBase {
    SYSTEM_DEF(PopEnemyBubbleSystem, SystemTypeFlags::GAMEPLAY)

  protected:
    void Update() override;
};
