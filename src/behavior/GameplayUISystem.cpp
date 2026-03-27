#include "GameplayUISystem.h"
#include "raylib.h"

void GameplayUISystem::Init() {
}

void GameplayUISystem::Update() {
    for (const Event &event : eventSystem.ReadEvent(POINTS_GAINED)) {
        int gainedPoints = event.data;
        points += gainedPoints;
    }

    // This could theoretically break, if the textEntity points to a different UIText entity
    // However this seems unlikely and for now this is good enough
    if (registry.valid(textEntity) && registry.any_of<ModifiableUITextComponent>(textEntity)) {
        registry.get<ModifiableUITextComponent>(textEntity).text = "HI SCORE\n" + std::to_string(points);

    } else if (eventSystem.ReadEvent(INSTANTIATE_GAME_UI).size()) {
        textEntity = EntityFactory::CreateModifiableUIText({BP_SIZE(26, 0), 0}, "HI SCORE\n" + std::to_string(points), GREEN);
    }

}
