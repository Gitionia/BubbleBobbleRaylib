#pragma once

#include "../app/EventSystem.h"

#include "../ecs/SystemBase.h"
#include <memory>

class SystemRunner {
  public:
    explicit SystemRunner(entt::registry &registry, EventSystem& eventSystem);
    ~SystemRunner() = default;

    void Init();
    void SetupAllSystems();
    void SetupOnlyEnabledSystems();
    void UpdateSystems() const;

    void OnlyHaveSystemsEnabledThatMatchAnyFlag(int flags);

    template <typename T>
    std::shared_ptr<SystemBase> CreateSystem();

  private:
    template <typename T>
    void registerSystem();

  private:
    entt::registry &registry;
    EventSystem& eventSystem;
    std::vector<std::shared_ptr<SystemBase>> systems;
};

