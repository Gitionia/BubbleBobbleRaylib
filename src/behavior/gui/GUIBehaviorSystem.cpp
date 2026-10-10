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
        focusedGUIElement = (entt::entity)events.at(0).data;

        if (!registry.valid(focusedGUIElement)) {
            focusedGUIElement = entt::null;
        }
    }

    // Try find any Button group
    if (focusedGUIElement == entt::null) {
        auto view = registry.view<GUIButtonGroup>();
        for (auto entity : view) {
            PRINT_WARN("No root GUI element was specified! Setting it now to {}", (uint64_t)entity);
            focusedGUIElement = entity;
            break;
        }
    }

    if (focusedGUIElement != entt::null) {
        updateFocusOfGUIButtonGroup();
    }
}

void GUIBehaviorSystem::updateFocusOfGUIButtonGroup() {
    if (registry.any_of<GUIButtonGroup>(focusedGUIElement)) {
        GUIButtonGroup &group = registry.get<GUIButtonGroup>(focusedGUIElement);

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

    if (focusedGUIElement == entt::null) {
        PRINT_WARN("No ButtonGroup is focused! Not simulating GUI");
        return;
    }

    GUIButtonGroup *group = registry.try_get<GUIButtonGroup>(focusedGUIElement);
    if (group) {
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
    }
}