#pragma once

#include "EntityFactory.h"

#define SYSTEM_DEF(Type)                                              \
  public:                                                             \
    explicit Type(entt::registry &registry) : SystemBase(registry) {} \
                                                                      \
  private:

class SpriteManager;
class SystemBase {
  public:
    static void BaseInit();

    SystemBase(entt::registry &registry);
    virtual ~SystemBase() = default;
    virtual void Init() {}

    void BaseUpdate();

  protected:
    virtual void Update() = 0;
    void Destroy(const entt::entity &e) const;

    using DeferFunctionType = std::function<void(entt::registry &, entt::entity)>;
    void Defer(entt::entity e, DeferFunctionType f, int index);

  protected:
    static constexpr int DEFER_MAX_COUNT = 5;
    static inline std::vector<std::vector<entt::entity>> deferedEntityCollections;
    static inline std::vector<DeferFunctionType> deferFunctions;
    entt::registry &registry;
};
