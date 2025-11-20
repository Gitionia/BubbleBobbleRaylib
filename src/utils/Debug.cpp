#include "Debug.h"

#include "Log.h"
#include "../ecs/Components.h"

struct DebugDrawTag;
struct Position;
struct DebugCircle;

void Debug::DrawPoint(int x, int y, float radius, Color color) {
	auto& registry = get().registry;
	auto point = registry->create();
	registry->emplace<DebugDrawTag>(point);
	registry->emplace<Position>(point, x, y);
	registry->emplace<DebugCircle>(point, radius, color);
}

void Debug::PrintNumberOfEntities() {
	int count = 0;
	for(auto entity: get().registry->view<entt::entity>())
	{
		count++;
	}

	PRINT_INFO("Number of entities: {}", count);
}

void Debug::setRegistry(entt::registry &_registry) {
	registry = &_registry;
}

Debug & Debug::get() {
	static Debug debug;
	return debug;
}
