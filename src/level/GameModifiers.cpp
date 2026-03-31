#include "GameModifiers.h"

static std::array<bool, (size_t)ModifierTypes::Count> g_modifierValues;

void GameModifierData::Init() {
    for (int i = 0; i < (int)ModifierTypes::Count; i++) {
        g_modifierValues.at(i) = false;
    }
}

void GameModifierData::Update() {
    for (const Event &event : eventSystem.ReadEvent(USING_GAME_MODIFIER)) {

        if (event.data >= 0 && event.data < (int)ModifierTypes::Count) {
            g_modifierValues.at(event.data) = true;

            PRINT_INFO("Picked up Modifier {}", event.data);
        }
    }
}

bool GameModifierData::IsModifierSet(ModifierTypes type) {
    if (type == ModifierTypes::Count) {
        DBG_CHECK(false, "Tried to check value of utility modifier value 'Count'");
        return false;
    }

    return g_modifierValues.at((int)type);
}
