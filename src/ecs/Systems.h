#pragma once

#include <entt/entt.hpp>

#include "../behavior/BubbleBehaviorSystem.h"
#include "../behavior/DragonBehaviorSystem.h"
#include "../graphics/RendererSystem.h"
#include "../graphics/Sprites.h"
#include "EntityFactory.h"

class SystemRunner {
  public:
    explicit SystemRunner(entt::registry &registry);
    ~SystemRunner();
    void Init();
    void UpdateSystems() const;

  private:
    template <typename T>
    void registerSystem(entt::registry &registry);

  private:
    entt::registry &registry;
    std::vector<SystemBase *> systems;
};
