#include "Debug.h"

#include "../ecs/Components.h"

struct DebugDrawTag;
struct Position;
struct DebugCircle;

void Debug::DrawPoint(int x, int y, float radius, Color color) {
	auto& registry = Get().registry;
	auto point = registry->create();
	registry->emplace<DebugDrawTag>(point);
	registry->emplace<Position>(point, x, y);
	registry->emplace<DebugCircle>(point, radius, color);
}

void Debug::PrintNumberOfEntities() {
	int count = 0;
	for(auto entity: Get().registry->view<entt::entity>())
	{
		count++;
	}

	printf("Number of entities: %d\n", count);
}

void Debug::setRegistry(entt::registry &_registry) {
	registry = &_registry;
}

Debug & Debug::Get() {
	static Debug debug;
	return debug;
}
