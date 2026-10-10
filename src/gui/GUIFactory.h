#pragma once

struct GUIButton {
    int Width;
    int Height;
    std::string Text;
    struct Color Color;
    int FontSize;
    int Spacing;

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
    static entt::entity CreateButton(int x, int y, int width, int height, const char *text, Color color = WHITE, int fontSize = 32, int spacing = 0);
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