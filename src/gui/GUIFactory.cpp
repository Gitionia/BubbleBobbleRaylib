#include "GUIFactory.h"

#include "../ecs/Components.h"

entt::entity GUIFactory::CreateButtonAutoSize(int x, int y, const char *text, Color unfocusedColor, Color focusedColor, Color backGroundColor, int fontSize, int spacing) {
    int backgroundOffsetX = -fontSize / 4;
    int backgroundOffsetY = -fontSize / 4;
    int width = (fontSize + spacing) / 2 * strlen(text) - 2 * backgroundOffsetX;
    int height = fontSize;
    return CreateButton(x, y, width, height, backgroundOffsetX, backgroundOffsetY, text, unfocusedColor, focusedColor, backGroundColor, fontSize, spacing);
}

entt::entity GUIFactory::CreateButton(int x, int y, int width, int height, int backgroundOffsetX, int backgroundOffsetY, const char *text, Color unfocusedColor, Color focusedColor, Color backGroundColor, int fontSize, int spacing) {
    entt::registry *registry = get().registry;
    entt::entity e = registry->create();

    static int debugEntityLabelNumber = 0;
    DBG_ADD_ENTITY_LABEL(registry, e, "GUI-Button-Entity", debugEntityLabelNumber++);

    registry->emplace<Position>(e, x, y);
    registry->emplace<GUIButton>(e, width, height, backgroundOffsetX, backgroundOffsetY, text, unfocusedColor, focusedColor, backGroundColor, fontSize, spacing);

    return e;
}

entt::entity GUIFactory::CreateButtonGroup(const std::vector<entt::entity> &buttons, GUILayout layout) {
    entt::registry *registry = get().registry;
    entt::entity e = registry->create();

    static int debugEntityLabelNumber = 0;
    DBG_ADD_ENTITY_LABEL(registry, e, "GUI-ButtonGroup-Entity", debugEntityLabelNumber++);

    registry->emplace<GUIButtonGroup>(e, layout, buttons);

    return e;
}

entt::entity GUIFactory::CreateDynamicText(int x, int y, std::shared_ptr<std::string> text, Color color, int fontSize, int spacing) {
    entt::registry *registry = get().registry;
    entt::entity e = registry->create();

    static int debugEntityLabelNumber = 0;
    DBG_ADD_ENTITY_LABEL(registry, e, "GUI-DynamicText-Entity", debugEntityLabelNumber++);

    registry->emplace<Position>(e, x, y);
    auto t = registry->emplace<GUIDynamicText>(e, text, color, fontSize, spacing);

    return e;
}

entt::entity GUIFactory::CreateStaticText(int x, int y, const std::string& text, Color color, int fontSize, int spacing) {
    entt::registry *registry = get().registry;
    entt::entity e = registry->create();

    static int debugEntityLabelNumber = 0;
    DBG_ADD_ENTITY_LABEL(registry, e, "GUI-StaticText-Entity", debugEntityLabelNumber++);

    registry->emplace<Position>(e, x, y);
    registry->emplace<GUIStaticText>(e, text, color, fontSize, spacing);

    return e;
}

GUIFactory &GUIFactory::get() {
    static GUIFactory guiFactory;
    return guiFactory;
}

void GUIFactory::setRegistry(entt::registry &registry) {
    GUIFactory::registry = &registry;
}

bool GUI::IsButtonPressed(entt::entity e) {
    entt::registry *registry = get().registry;

    if (registry->any_of<GUIButton>(e)) {
        GUIButton button = registry->get<GUIButton>(e);
        return button.IsPressed;

    } else {
        return false;
    }
}

GUI &GUI::get() {
    static GUI gui;
    return gui;
}

void GUI::setRegistry(entt::registry &registry) {
    GUI::registry = &registry;
}