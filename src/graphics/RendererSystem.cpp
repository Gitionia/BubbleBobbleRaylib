#include "RendererSystem.h"
#include <raylib.h>
#include "../ecs/Components.h"

RendererSystem::RendererSystem(entt::registry& registry)
	: registry(registry)
{
}

void RendererSystem::update() const
{
	const int ballRadius = 30;

	BeginDrawing();

	ClearBackground(RAYWHITE);

	auto viewRenderer = registry.view<Position, BallSize, RenderData>();
	for (auto entity : viewRenderer) {
		auto [pos, size, renderData] = viewRenderer.get(entity);

		//DrawCircleV(Vector2(pos.x, pos.y), (float)size.radius, MAROON);

		DrawTextureRec(renderData.sprite.spriteSheet, renderData.sprite.coords, { (float)pos.x, (float)pos.y}, WHITE);
	}

	DrawFPS(10, 10);

	EndDrawing();
}
