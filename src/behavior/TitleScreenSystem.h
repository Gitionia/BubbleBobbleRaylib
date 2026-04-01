#pragma once

#include "../ecs/SystemBase.h"
#include <memory>

class TitleScreenSystem : public SystemBase {
    SYSTEM_DEF(TitleScreenSystem, SystemTypeFlags::TITLE_SCREEN)

  protected:
    void Setup() override;
    void Update() override;

  private:
    entt::entity title;
    entt::entity anyKey;
    int timeCounter = 0;
};