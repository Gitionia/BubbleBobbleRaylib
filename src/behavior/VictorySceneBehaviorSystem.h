#pragma once

#include "../ecs/SystemBase.h"

class VictorySceneBehaviorSystem : public SystemBase {
    SYSTEM_DEF(VictorySceneBehaviorSystem, SystemTypeFlags::VICTORY_SCENE)
  public:
    void Init() override;
    void Setup() override;

  protected:
    void Update() override;

  private:
};
