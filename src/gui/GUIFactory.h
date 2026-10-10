#pragma once

struct GUIButton {
    int Width;
    int Height;
    int BackgroundOffsetX;
    int BackgroundOffsetY;
    std::string Text;
    Color UnfocusedColor;
    Color FocusedColor;
    Color BackgroundColor;

    int FontSize;
    int Spacing;

    bool IsFocused = false;
    bool IsPressed = false;
};

enum class GUILayout {
    HORIZONTAL = 0,
    VERTICAL = 1,
};

struct GUIDynamicText {
    std::shared_ptr<std::string> Text;
    Color TextColor;
    int FontSize;
    int Spacing;
};

struct GUIStaticText {
    std::string Text;
    Color TextColor;
    int FontSize;
    int Spacing;
};

struct GUIButtonGroup {
    GUILayout Layout;
    const std::vector<entt::entity> Buttons;
    int FocusedButtonIndex = 0;

    bool IsFocusedIndexValid() {
        return FocusedButtonIndex >= 0 && FocusedButtonIndex < Buttons.size();
    }

    bool IncreaseFocusIndex(int increase) {
        if (IsFocusedIndexValid()) {
            FocusedButtonIndex = (FocusedButtonIndex + increase + Buttons.size()) % Buttons.size();
            return true;
        }
        return false;
    }
};

class GUIFactory {
  public:
    static entt::entity CreateButtonAutoSize(int x, int y, const char *text, Color unfocusedColor, Color focusedColor, Color backgroundColor, int fontSize = 32, int spacing = 0);
    static entt::entity CreateButton(int x, int y, int width, int height, int backgroundOffsetX, int backgroundOffsetY, const char *text, Color unfocusedColor, Color focusedColor, Color backgroundColor, int fontSize = 32, int spacing = 0);
    static entt::entity CreateButtonGroup(const std::vector<entt::entity> &buttons, GUILayout layout);
    static entt::entity CreateDynamicText(int x, int y, std::shared_ptr<std::string> text, Color color, int fontSize = 32, int spacing = 0);
    static entt::entity CreateStaticText(int x, int y, const std::string& text, Color color, int fontSize = 32, int spacing = 0);

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