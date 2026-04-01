#include "Debug.h"

#include "../ecs/Components.h"
#include "raylib.h"

struct DebugDrawTag;
struct Position;
struct DebugCircle;

void Debug::DrawPoint(int x, int y, float radius, Color color) {
    auto &registry = get().registry;
    auto point = registry->create();

    static int debugEntityLabelNumber = 0;
    DBG_ADD_ENTITY_LABEL(registry, point, "DEBUG_POINT_DRAWING", debugEntityLabelNumber++);

    registry->emplace<DebugDrawTag>(point);
    registry->emplace<Position>(point, x, y);
    registry->emplace<DebugCircle>(point, radius, color);
}

void Debug::DrawCollider(int x, int y, Collider collider, Color color) {
    auto &registry = get().registry;
    auto colliderEntity = registry->create();

    static int debugEntityLabelNumber = 0;
    DBG_ADD_ENTITY_LABEL(registry, colliderEntity, "DEBUG_COLLIDER_DRAWING", debugEntityLabelNumber++);

    registry->emplace<DebugDrawTag>(colliderEntity);
    registry->emplace<Position>(colliderEntity, x + collider.offsetX, y + collider.offsetY);
    registry->emplace<DebugRectangle>(colliderEntity, collider.width, collider.height, color);
}

void Debug::PrintNumberOfEntities() {
    int count = 0;
    for (auto entity : get().registry->view<entt::entity>()) {
        count++;
    }

    PRINT_INFO("Number of entities: {}", count);
}

void Debug::PrintAllDebuggingLabels() {
    for (auto entity : get().registry->view<DebugEntityLabelComponent>()) {
        auto debugLabel = get().registry->get<DebugEntityLabelComponent>(entity);
        PRINT_INFO("Entity: {}", debugLabel.label);
    }
    PrintNumberOfEntities();
}

void Debug::PrintMousePosition() {
    PRINT_INFO("X: {}, Y: {}", GetMousePosition().x, GetMousePosition().y);
}

void Debug::setRegistry(entt::registry &_registry) {
    registry = &_registry;
}

Debug &Debug::get() {
    static Debug debug;
    return debug;
}
