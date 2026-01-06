#pragma once

#include "../ecs/SystemBase.h"

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
