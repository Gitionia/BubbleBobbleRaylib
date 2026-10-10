#pragma once

#include "../app/EventSystem.h"

namespace SystemTypeFlags {
static inline constexpr int ALL = 0xFFFFFFFF;
static inline constexpr int RENDERING = 1 << 0;
static inline constexpr int GAMEPLAY = 1 << 1;
static inline constexpr int LEVEL_INSTANTIATION = 1 << 2;
static inline constexpr int GAME_UI = 1 << 3;
static inline constexpr int TITLE_SCREEN = 1 << 4;
static inline constexpr int GAME_MODIFIER = 1 << 5;
static inline constexpr int INTRO_SCENE = 1 << 6;
static inline constexpr int VICTORY_SCENE = 1 << 7;
} // namespace SystemTypeFlags

#define SYSTEM_DEF(Type, typeFlags)                                   \
  public:                                                             \
    explicit Type(entt::registry &registry, EventSystem &eventSystem) \
        : SystemBase(registry, eventSystem, typeFlags) {}             \
                                                                      \
  private:

class SpriteManager;
class SystemBase {
  public:
    static void BaseInit();

    SystemBase(entt::registry &registry, EventSystem &eventSystem, int typeFlags);
    virtual ~SystemBase() = default;
    // Is called once after libraries have been initialized.
    // Can be used to get references to resources
    virtual void Init() {}
    // Is called before Update() gets called and can be used to create entities.
    // A System can be Setup multiple times without having to be destroyed.
    virtual void Setup() {}
    // Is called usually when a state is exited and a system needs clean up objects that were
    // created in Setup()
    virtual void CleanUp() {}

    void BaseUpdate();

    void SetEnabledIfMatchesAnyFlag(int flags);

    bool IsEnabled();
    void SetEnabled(bool enabled);

  protected:
    virtual void Update() = 0;
    void Destroy(const entt::entity &e) const;

    using DeferFunctionType = std::function<void(entt::registry &, entt::entity)>;
    void Defer(entt::entity e, DeferFunctionType f, int index);

  protected:
    static constexpr int DEFER_MAX_COUNT = 5;
    static inline std::array<std::vector<entt::entity>, DEFER_MAX_COUNT> deferedEntityCollections{};
    static inline std::array<DeferFunctionType, DEFER_MAX_COUNT> deferFunctions{};

    entt::registry &registry;
    EventSystem &eventSystem;

    int typeFlags;

  private:
    bool enabled = false;
};
