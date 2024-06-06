#include "RendererSystem.h"
#include <raylib.h>
#include "../ecs/Components.h"

RendererSystem::RendererSystem(entt::registry& registry)
	: registry(registry), mainSpriteSheet(LoadTexture("res/MainSpriteSheet.png"))
{
}

RendererSystem::~RendererSystem()
{
	UnloadTexture(mainSpriteSheet);
}

void RendererSystem::update()
{
	const int ballRadius = 30;

	BeginDrawing();

	ClearBackground(RAYWHITE);

	auto viewRenderer = registry.view<Position, BallSize, Sprite>();
	for (auto entity : viewRenderer) {
		auto [pos, size, sprite] = viewRenderer.get(entity);

		//DrawCircleV(Vector2(pos.x, pos.y), (float)size.radius, MAROON);

		DrawTextureRec(mainSpriteSheet, sprite.coords, { (float)pos.x, (float)pos.y}, WHITE);
	}

	DrawFPS(10, 10);

	EndDrawing();
}
