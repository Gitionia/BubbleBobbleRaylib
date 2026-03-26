#pragma once

class Debug {

  public:
    static void DrawPoint(int x, int y, float radius, Color color);
    static void PrintNumberOfEntities();

  private:
    void setRegistry(entt::registry &_registry);
    static Debug &get();
    entt::registry *registry;

    friend class Application;
};


#ifdef NDEBUG
#define DBG_ASSERT(x)
#else
#define DBG_ASSERT(x)                                                   \
    if (!(x)) {                                                         \
        PRINT_ERROR("Assert failed at {} line {}", __FILE__, __LINE__); \
        exit(0);                                                        \
    }
#endif

#ifdef NDEBUG
#define DBG_CHECK(x, msg)
#else
#define DBG_CHECK(x, msg)                                                                           \
    if (!(x)) {                                                                                     \
        PRINT_ERROR("Check failed at {} line {}. Continuing. Message {}", __FILE__, __LINE__, msg); \
    }
#endif
