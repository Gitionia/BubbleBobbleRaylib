#include "RendererSystem.h"
#include <raylib.h>
#include <raymath.h>
#include "../ecs/Components.h"

#include "../Utilities.h"

template<typename Tag>
void renderAllWithTag(const entt::registry& registry) {
	auto viewRenderer = registry.view<Position, RenderData, Tag>();
	for (auto entity : viewRenderer) {
		auto [pos, renderData] = viewRenderer.get(entity);

		//DrawCircleV(Vector2(pos.x, pos.y), (float)size.radius, MAROON);

		// DrawTextureRec(renderData.sprite.spriteSheet, renderData.sprite.coords, { (float)pos.x, (float)pos.y}, WHITE);

		DrawTexturePro(renderData.sprite.spriteSheet, renderData.GetCoordsWithOrientation(),
			ScaleRect({ (float)pos.x, (float)pos.y, renderData.sprite.coords.width * renderData.scale.x, renderData.sprite.coords.height * renderData.scale.y}, RendererSystem::SCALE_SIZE),
			{0,0}, 0, WHITE);
	}
}

void RendererSystem::Update()
{
	const int ballRadius = 30;

	BeginDrawing();

	ClearBackground(BLACK);


	renderAllWithTag<LevelTileTag>(registry);
	renderAllWithTag<DragonTag>(registry);

	DrawFPS(10, 10);

	EndDrawing();
}
