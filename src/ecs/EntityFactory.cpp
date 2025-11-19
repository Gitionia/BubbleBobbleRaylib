#include "EntityFactory.h"

#include "Components.h"

EntityFactory::EntityFactory(entt::registry& registry)
        : registry(registry)
{ 
}

entt::entity EntityFactory::CreateTile(int x, int y, Color colorShadowRight, Color colorShadowBottem, bool addShadowRight, bool addShadowBottem) const {
	auto tile = registry.create();
	{
		registry.emplace<Position>(tile, x * UNITS_PER_BLOCK, y * UNITS_PER_BLOCK);
		RenderData data = {GetSpriteHandle("Level1"), {1, 1}};
		registry.emplace<RenderData>(tile, data);
		registry.emplace<LevelTileTag>(tile);
		registry.emplace<Collider>(tile, UNITS_PER_BLOCK, UNITS_PER_BLOCK, 0, 0);
	}

	if (addShadowRight)
	{
		auto shadowRight = registry.create();
		registry.emplace<Position>(shadowRight, (x + 1) * UNITS_PER_BLOCK, y * UNITS_PER_BLOCK);
		RenderData data = {GetSpriteHandle("TileShadowRight"), {2, 2}};
		data.SetColor(colorShadowRight);
		registry.emplace<RenderData>(shadowRight, data);
		registry.emplace<LevelTileShadowTag>(shadowRight);
	}
	if (addShadowBottem)
	{
		auto shadowBottem = registry.create();
		registry.emplace<Position>(shadowBottem, x * UNITS_PER_BLOCK, (y + 1) * UNITS_PER_BLOCK);
		RenderData data = {GetSpriteHandle("TileShadowBottem"), {2, 2}};
		data.SetColor(colorShadowBottem);
		registry.emplace<RenderData>(shadowBottem, data);
		registry.emplace<LevelTileShadowTag>(shadowBottem);
	}

	return tile;
}

entt::entity EntityFactory::CreateDragon() const
{
    auto dragon = registry.create();
	registry.emplace<Position>(dragon, 5 * UNITS_PER_BLOCK, 5 * UNITS_PER_BLOCK);
	registry.emplace<RenderData>(dragon, RenderData(GetSpriteHandle("Dragon-Idle-1"), {2, 2}));
	registry.emplace<Collider>(dragon, 2 * UNITS_PER_BLOCK, UNITS_PER_BLOCK, 0, UNITS_PER_BLOCK);
    // Spikes on the back have full height and 6 pixels to the end of the sprite
	registry.emplace<DragonSpikeCollider>(dragon, 12 * UNITS_PER_PIXEL, 2 * UNITS_PER_BLOCK - 4 * UNITS_PER_PIXEL, 2 * UNITS_PER_BLOCK - 12 * UNITS_PER_PIXEL, 0,
													2 * UNITS_PER_BLOCK, 4 * UNITS_PER_PIXEL, 0 * UNITS_PER_BLOCK, 2 * UNITS_PER_BLOCK - 6 * UNITS_PER_PIXEL);
	registry.emplace<WalkingActorComponent>(dragon);
	registry.emplace<DragonTag>(dragon);

    return dragon;
}

entt::entity EntityFactory::CreateBubble(const Position& pos, int direction) const {
	auto bubble = registry.create();
	registry.emplace<Position>(bubble, pos);
	registry.emplace<RenderData>(bubble, RenderData(GetSpriteHandle("Bubble-Green-Idle-1"), {2, 2}).SetDirection(direction));
	registry.emplace<Collider>(bubble, 2 * (UNITS_PER_BLOCK / 16) * 14, 2 * UNITS_PER_BLOCK, 0, 0);
	registry.emplace<BubbleComponent>(bubble, direction, Animator(&Animations::Get().GetAnimation("Bubble-Green-Idle")));
	registry.emplace<BubbleTag>(bubble);

	return bubble;
}

void EntityFactory::CreateLevel(LevelLayout &level) const
{
	for (int x = 2; x < LevelTilemap::WIDTH + 2; ++x) {
		for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
			bool addShadowRight = LevelTilemap::OutOfRange((x - 2) + 1, y) ? true : level.GetTiles().IsEmpty((x - 2) + 1, y);
			bool addShadowBottem = LevelTilemap::OutOfRange((x - 2), y + 1) ? true : level.GetTiles().IsEmpty((x - 2), y + 1);
			if (!level.GetTiles().IsEmpty(x - 2, y)) {
				CreateTile(x, y, level.GetShadeColorRight(), level.GetShadeColorBottem()
					, addShadowRight, addShadowBottem);
			}
	}
}

	int rows[] = {0, 1, 30, 31};
	for (int x : rows) {
		for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
			bool addShadowRight = x == 1;
			bool addShadowBottem = false;
			CreateTile(x, y, level.GetShadeColorRight(), level.GetShadeColorBottem()
				, addShadowRight, addShadowBottem);
		}
	}
}
