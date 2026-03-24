#pragma once

#include "../app/Config.h"
#include "../ecs/SystemBase.h"

class RendererSystem : public SystemBase {
    SYSTEM_DEF(RendererSystem)

  public:
    void Init() override;
    ~RendererSystem();

  protected:
    void Update() override;

  private:
    template <typename Tag>
    void renderAllWithTag();

    void renderFontText();

    template <typename ColliderType>
    void debugDrawColliders(Color color);

    template <typename ColliderType>
    void debugDrawMultiColliders(Color color);

  private:
    void drawDebugShapes();

  private:
  RenderTexture2D renderTexture;
  Font font;
};
