#pragma once

#include "entt/entity/fwd.hpp"
#include <vector>
struct GUIButton {
    int Width;
    int Height;
    std::string Text;

    bool IsFocused = false;
    bool IsPressed = false;
};

enum class GUILayout {
    HORIZONTAL = 0,
    VERTICAL = 1,
};

struct GUIButtonGroup {
    GUILayout Layout;
    const std::vector<entt::entity> Buttons;
};

class GUIFactory {
  public:
    static entt::entity CreateButton(int x, int y, int width, int height, const char *text);
    static entt::entity CreateButtonGroup(const std::vector<entt::entity> &buttons, GUILayout layout);

  private:
    static GUIFactory &get();
    void setRegistry(entt::registry &registry);
    entt::registry *registry;

    friend class Application;
};

class GUI {
  public:
    static bool IsButtonPressed(entt::entity button);

  private:
    static GUI &get();
    void setRegistry(entt::registry &registry);
    entt::registry *registry;

    friend class Application;
};