#include "GameplayUISystem.h"

void GameplayUISystem::Init() {
    textEntity = EntityFactory::CreateModifiableUIText({BP_SIZE(26, 0), 0}, "HI SCORE\n" + std::to_string(points), GREEN);

    ui = &registry.get<ModifiableUITextComponent>(textEntity);
}

void GameplayUISystem::Update() {
    for (const Event &event : eventSystem.ReadEvent(POINTS_GAINED)) {
        int gainedPoints = event.data;
        points += gainedPoints;
    }

    ui->text = "HI SCORE\n" + std::to_string(points);
}
