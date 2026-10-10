#include "GUIBehaviorSystem.h"

#include "../../ecs/Components.h"
#include "../../gui/GUIFactory.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <cstdint>

void GUIBehaviorSystem::Setup() {
    std::vector<Event> events = eventSystem.ReadEvent(SET_GUI_FOCUS_TO_ENTITY);
    if (!events.empty()) {
        guiButtonGroup = (entt::entity)events.at(0).data;

        if (!registry.valid(guiButtonGroup)) {
            guiButtonGroup = entt::null;
        }
    }

    // Try find any Button group
    if (guiButtonGroup == entt::null) {
        auto view = registry.view<GUIButtonGroup>();
        for (auto entity : view) {
            PRINT_WARN("No root GUI element was specified! Setting it now to {}", (uint64_t)entity);
            guiButtonGroup = entity;
            break;
        }
    }

    if (guiButtonGroup != entt::null) {
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

void GUIBehaviorSystem::Update() {
    if (pressedGUIButton != entt::null) {
        GUIButton *button = registry.try_get<GUIButton>(pressedGUIButton);
        if (button) {
            button->IsPressed = false;
            pressedGUIButton = entt::null;
        }
    }

    if (guiButtonGroup == entt::null) {
        PRINT_WARN("No ButtonGroup is focused! Not simulating GUI");
        return;
    }

    GUIButtonGroup *group = registry.try_get<GUIButtonGroup>(guiButtonGroup);
    if (!group)
        return;
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

    if (Input::IsKeyDown(Key::Fire, DRAGON_GREEN) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
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