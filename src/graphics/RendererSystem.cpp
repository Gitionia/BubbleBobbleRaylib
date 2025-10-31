#include "RendererSystem.h"
#include <raylib.h>
#include <raymath.h>
#include "../ecs/Components.h"

#include "../utils/Utilities.h"
#include "../app/Config.h"

void RendererSystem::Update()
{
	BeginDrawing();

	ClearBackground(BLACK);

	renderAllWithTag<LevelTileTag>();
	renderAllWithTag<BubbleTag>();
	renderAllWithTag<DragonTag>();

#ifdef DRAW_DEBUG
	debugDrawColliders<Collider>(RED);
	debugDrawColliders<DragonSpikeCollider>(BLUE);
	debugDrawColliders<BubbleJumpableTopCollider>(GREEN);
#endif

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
			ScaleRect({ (float)pos.x / UNITS_TO_PIXEL_SCALE + renderData.xoffset, (float)pos.y / UNITS_TO_PIXEL_SCALE + renderData.yoffset, sprite.coords.width * renderData.scale.x, sprite.coords.height * renderData.scale.y }, SCALING_FACTOR),
			{0,0}, 0, WHITE);
	}
}


template<typename ColliderType>
void RendererSystem::debugDrawColliders(Color color) {
	auto viewRenderer = registry.view<Position, ColliderType>();
	for (auto entity : viewRenderer) {
		auto [pos, col] = viewRenderer.get(entity);

        // * 2, because one Block has 32 screen pixels, but has 16 sprite pixels
		DrawRectangle((pos.x + col.offsetX) * 2, (pos.y + col.offsetY) * 2, 
            col.width * 2, col.height * 2, color);
	}
}