#pragma once

#include "EntityFactory.h"

#include "../app/EventSystem.h"

namespace SystemTypeFlags {
static inline constexpr int RENDERING = 1 << 0;
static inline constexpr int GAMEPLAY = 1 << 1;
static inline constexpr int LEVEL_INSTANTIATION = 1 << 2;
static inline constexpr int GAME_UI = 1 << 3;
static inline constexpr int TITLE_SCREEN = 1 << 4;
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
    virtual void Init() {}

    void BaseUpdate();

    void SetEnabledIfMatchesAnyFlag(int flags);

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
