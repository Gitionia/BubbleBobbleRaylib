#pragma once

#include "../ecs/SystemBase.h"

class PopEnemyBubbleSystem: public SystemBase {
    SYSTEM_DEF(PopEnemyBubbleSystem)

  protected:
    void Update() override;
};
