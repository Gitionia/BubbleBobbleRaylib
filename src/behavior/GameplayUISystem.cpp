#include "GameplayUISystem.h"
#include "raylib.h"

void GameplayUISystem::Init() {
}

void GameplayUISystem::Update() {
    for (const Event &event : eventSystem.ReadEvent(POINTS_GAINED)) {
        int gainedPoints = event.data;
        points += gainedPoints;
    }

    if (eventSystem.ReadEvent(INSTANTIATE_LEVEL).size()) {
        Event event = eventSystem.ReadEvent(INSTANTIATE_LEVEL).at(0);
        level = event.data;
    }

    if (eventSystem.ReadEvent(DELETE_GAMEPLAY).size()) {
        auto view = registry.view<UITextEntityTag>();
        registry.destroy(view->begin(), view->end());
    }

    // This could theoretically break, if the textEntity points to a different UIText entity
    // However this seems unlikely and for now this is good enough
    if (registry.valid(scoreEntity) && registry.any_of<ModifiableUITextComponent>(scoreEntity)) {
        registry.get<ModifiableUITextComponent>(scoreEntity).text = "HI SCORE\n" + std::to_string(points);

    } else if (eventSystem.ReadEvent(INSTANTIATE_GAME_UI).size()) {
        scoreEntity = EntityFactory::CreateModifiableUIText({BP_SIZE(26, 0) / 2, 0}, "HI SCORE\n" + std::to_string(points), GREEN);
    }

    if (registry.valid(levelCounterInCorner) && registry.any_of<ModifiableUITextComponent>(levelCounterInCorner)) {
        registry.get<ModifiableUITextComponent>(levelCounterInCorner).text = std::to_string(level);

    } else if (eventSystem.ReadEvent(INSTANTIATE_GAME_UI).size()) {
        levelCounterInCorner = EntityFactory::CreateModifiableUIText({BP_SIZE(0, 0), BP_SIZE(0, 2)}, std::to_string(level), WHITE);
    }
}
