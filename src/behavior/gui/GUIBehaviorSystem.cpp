#include "GUIBehaviorSystem.h"

#include "../../ecs/Components.h"
#include "../../gui/GUIFactory.h"
#include "entt/entity/entity.hpp"

void GUIBehaviorSystem::Setup() {
    setFocusedGUIButtonGroupFromEventSystem();
}

void GUIBehaviorSystem::setFocusedGUIButtonGroupFromEventSystem() {
    {
        const std::vector<Event> &events = eventSystem.ReadEvent(SET_ACTIVE_GUI_BUTTON_GROUP);
        if (!events.empty()) {
            if (registry.valid(guiButtonGroup)) {
                unfocusAllButtonsInButtonGroup();
            }

            guiButtonGroup = (entt::entity)events.at(0).data;

            if (!registry.valid(guiButtonGroup)) {
                guiButtonGroup = entt::null;
            }
        }
    }

    if (guiButtonGroup != entt::null) {
        const std::vector<Event> &events = eventSystem.ReadEvent(SET_FOCUSED_GUI_BUTTON);
        if (!events.empty()) {

            if (registry.any_of<GUIButtonGroup>(guiButtonGroup)) {
                GUIButtonGroup &group = registry.get<GUIButtonGroup>(guiButtonGroup);
                group.FocusedButtonIndex = (int)events.at(0).data;

                if (!group.IsFocusedIndexValid()) {
                    group.FocusedButtonIndex = 0;
                }
            }
        }

        updateFocusOfGUIButtonGroup();
    }
}

void GUIBehaviorSystem::updateFocusOfGUIButtonGroup() {
    if (registry.any_of<GUIButtonGroup>(guiButtonGroup)) {
        GUIButtonGroup &group = registry.get<GUIButtonGroup>(guiButtonGroup);

        if (!group.IsFocusedIndexValid()) {
            group.FocusedButtonIndex = 0;
        }

        for (int i = 0; i < group.Buttons.size(); i++) {
            entt::entity buttonEntity = group.Buttons.at(i);
            bool isFocused = i == group.FocusedButtonIndex;

            GUIButton *button = registry.try_get<GUIButton>(buttonEntity);
            if (button) {
                button->IsFocused = isFocused;
            }
        }
    }
}

void GUIBehaviorSystem::unfocusAllButtonsInButtonGroup() {
    if (registry.any_of<GUIButtonGroup>(guiButtonGroup)) {
        GUIButtonGroup &group = registry.get<GUIButtonGroup>(guiButtonGroup);

        group.FocusedButtonIndex = 0;

        for (int i = 0; i < group.Buttons.size(); i++) {
            entt::entity buttonEntity = group.Buttons.at(i);

            GUIButton *button = registry.try_get<GUIButton>(buttonEntity);
            if (button) {
                button->IsFocused = false;
            }
        }
    }
}

void GUIBehaviorSystem::Update() {
    if (pressedGUIButton != entt::null) {
        GUIButton *button = registry.try_get<GUIButton>(pressedGUIButton);
        if (button) {
            button->IsPressed = false;
            pressedGUIButton = entt::null;
        }
    }

    setFocusedGUIButtonGroupFromEventSystem();

    if (guiButtonGroup == entt::null) {
        return;
    }

    GUIButtonGroup *group = registry.try_get<GUIButtonGroup>(guiButtonGroup);
    if (!group)
        return;

    if (eventSystem.ReadEvent(UNFOCUS_GUI_BUTTON_GROUP).size()) {
        unfocusAllButtonsInButtonGroup();
        guiButtonGroup = entt::null;
        return;
    }

    if (group->Layout == GUILayout::VERTICAL) {
        if (IsKeyPressed(KEY_DOWN)) {
            if (group->IncreaseFocusIndex(1)) {
                updateFocusOfGUIButtonGroup();
            }
        } else if (IsKeyPressed(KEY_UP)) {
            if (group->IncreaseFocusIndex(-1)) {
                updateFocusOfGUIButtonGroup();
            }
        }
    } else if (group->Layout == GUILayout::HORIZONTAL) {
        if (IsKeyPressed(KEY_RIGHT)) {
            if (group->IncreaseFocusIndex(1)) {
                updateFocusOfGUIButtonGroup();
            }
        } else if (IsKeyPressed(KEY_LEFT)) {
            if (group->IncreaseFocusIndex(-1)) {
                updateFocusOfGUIButtonGroup();
            }
        }
    }

    if (Input::IsKeyDown(Key::Jump, DRAGON_GREEN) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (group->IsFocusedIndexValid()) {
            entt::entity buttonEntity = group->Buttons.at(group->FocusedButtonIndex);
            GUIButton *button = registry.try_get<GUIButton>(buttonEntity);
            if (button) {
                button->IsPressed = true;
                pressedGUIButton = buttonEntity;
            }
        }
    }
}