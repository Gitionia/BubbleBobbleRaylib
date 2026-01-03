#pragma once

#include "../app/Config.h"
#include "../ecs/SystemBase.h"
#include "entt/entt.hpp"
#include "raylib.h"

class RendererSystem : public SystemBase {
    SYSTEM_DEF(RendererSystem)

  protected:
    void Update() override;

  private:
    template <typename Tag>
    void renderAllWithTag();

    template <typename ColliderType>
    void debugDrawColliders(Color color);

    template <typename ColliderType>
    void debugDrawMultiColliders(Color color);

  private:
    void drawDebugShapes();
};
