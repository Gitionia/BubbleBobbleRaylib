#pragma once

#include "../../ecs/SystemBase.h"

class MenuBehaviorSystem : public SystemBase {
    SYSTEM_DEF(MenuBehaviorSystem, 0)
  public:
    void Setup() override;
    void CleanUp() override;

  protected:
    void Update() override;

  private:
    entt::entity button1Player;
    entt::entity button2Players;
    entt::entity buttonTest;
    entt::entity buttonGroup;
};
