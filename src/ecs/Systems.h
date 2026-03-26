#pragma once

#include "../app/EventSystem.h"

#include "../ecs/SystemBase.h"

class SystemRunner {
  public:
    explicit SystemRunner(entt::registry &registry, EventSystem& eventSystem);
    ~SystemRunner();
    void Init();
    void UpdateSystems() const;

    void OnlyHaveSystemsEnabledThatMatchAnyFlag(int flags);

  private:
    template <typename T>
    void registerSystem(entt::registry &registry, EventSystem& eventSystem);

  private:
    entt::registry &registry;
    std::vector<SystemBase *> systems;
};
