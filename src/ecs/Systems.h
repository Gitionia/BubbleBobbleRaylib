#pragma once

#include "../app/EventSystem.h"

#include "../ecs/SystemBase.h"

class SystemRunner {
  public:
    explicit SystemRunner(entt::registry &registry, EventSystem &eventSystem);
    ~SystemRunner() = default;

    void Init();
    void SetupAllSystems();
    void SetupOnlyEnabledSystems();
    void UpdateSystems() const;

    void OnlyHaveSystemsEnabledThatMatchAnyFlag(int flags);

    template <typename T>
    std::shared_ptr<SystemBase> CreateSystem() {
        std::shared_ptr<T> system = std::make_shared<T>(registry, eventSystem);
        return system;
    }

    std::shared_ptr<SystemBase> GetRendererSystem();

  private:
    template <typename T>
    std::shared_ptr<SystemBase> registerSystem();

  private:
    entt::registry &registry;
    EventSystem &eventSystem;
    std::vector<std::shared_ptr<SystemBase>> systems;
    std::shared_ptr<SystemBase> rendererSystem;
};
