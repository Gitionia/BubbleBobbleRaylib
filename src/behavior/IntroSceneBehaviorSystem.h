#pragma once

#include "../ecs/SystemBase.h"

class IntroSceneBehaviorSystem : public SystemBase {
    SYSTEM_DEF(IntroSceneBehaviorSystem, SystemTypeFlags::INTRO_SCENE)
  public:
    void Init() override;
    void Setup() override;

  protected:
    void Update() override;
};
