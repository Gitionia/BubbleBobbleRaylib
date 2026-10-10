#include "GUIFactory.h"

#include "../ecs/Components.h"

entt::entity GUIFactory::CreateButton(int x, int y, int width, int height, const char *text, Color color, int fontSize, int spacing) {
    entt::registry *registry = get().registry;
    entt::entity e = registry->create();

    static int debugEntityLabelNumber = 0;
    DBG_ADD_ENTITY_LABEL(registry, e, "GUI-Button-Entity", debugEntityLabelNumber++);

    registry->emplace<Position>(e, x, y);
    registry->emplace<GUIButton>(e, width, height, text, color, fontSize, spacing);

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

GUIFactory& GUIFactory::get() {
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

GUI& GUI::get() {
    static GUI gui;
    return gui;
}

void GUI::setRegistry(entt::registry &registry) {
    GUI::registry = &registry;
}