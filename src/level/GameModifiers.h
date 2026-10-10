#pragma once

#include "../ecs/SystemBase.h"

enum class ModifierTypes {

    SPEED_UP,
    FIRERATE_UP,
    RANGE_UP,
    Count
};

// Gives access to applied modfiers, such as powerups
class GameModifierData : public SystemBase {
    SYSTEM_DEF(GameModifierData, SystemTypeFlags::GAME_MODIFIER | SystemTypeFlags::GAMEPLAY)

  public:
    void Init() override;

    static bool IsModifierSet(ModifierTypes type);

  protected:
    void Update() override;
};
