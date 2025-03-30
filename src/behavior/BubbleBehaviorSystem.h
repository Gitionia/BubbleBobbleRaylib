#pragma once

#include "../ecs/SystemBase.h"

class BubbleBehaviorSystem : public SystemBase {
    SYSTEM_DEF(BubbleBehaviorSystem)

public:
    void Update() override;

};