#pragma once

#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#pragma once

#include "../../ecs/SystemBase.h"

class GUIBehaviorSystem : public SystemBase {
    SYSTEM_DEF(GUIBehaviorSystem, 0)

  public:
    void Setup() override;

  protected:
    void Update() override;

  private:
    void updateFocusOfGUIButtonGroup();

  private:
    entt::entity guiButtonGroup = entt::null;
    entt::entity pressedGUIButton = entt::null;
};
