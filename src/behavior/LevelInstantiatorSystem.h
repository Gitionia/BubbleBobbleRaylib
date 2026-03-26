#pragma once

#include "../ecs/SystemBase.h"
#include "../level/Level.h"

class LevelInstantiatorSystem : public SystemBase {
    SYSTEM_DEF(LevelInstantiatorSystem, SystemTypeFlags::LEVEL_INSTANTIATION)

  protected:
    void Update() override;

  private:
    void clearExistingLevel();
    void loadNewLevel(int newLevel);

  private:
    LevelLayout level;
};
