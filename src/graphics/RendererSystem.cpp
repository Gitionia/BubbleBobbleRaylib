#include "RendererSystem.h"
#include <raylib.h>
#include <raymath.h>
#include "../ecs/Components.h"

#include "../Utilities.h"

void RendererSystem::Update()
{
	const int ballRadius = 30;

	BeginDrawing();

	ClearBackground(BLACK);

	renderAllWithTag<LevelTileTag>();
	renderAllWithTag<DragonTag>();

	DrawFPS(10, 10);

	EndDrawing();
}

template<typename Tag>
void RendererSystem::renderAllWithTag() {
	auto viewRenderer = registry.view<Position, RenderData, Tag>();
	for (auto entity : viewRenderer) {
		auto [pos, renderData] = viewRenderer.get(entity);
		const Sprite& sprite = spriteManager.GetSprite(renderData.spriteHandle);
		Rectangle sourceRect = sprite.coords;
		if (renderData.flipX) sourceRect.width *= -1;
		if (renderData.flipY) sourceRect.height *= -1;

		DrawTexturePro(*sprite.spriteSheet, sourceRect,
			ScaleRect({ (float)pos.x, (float)pos.y, sprite.coords.width * renderData.scale.x, sprite.coords.height * renderData.scale.y}, RendererSystem::SCALE_SIZE),
			{0,0}, 0, WHITE);
	}
}