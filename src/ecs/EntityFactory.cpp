#include "EntityFactory.h"

#include "Components.h"

EntityFactory::EntityFactory(entt::registry& registry, const SpriteManager& spriteManager) 
        : registry(registry), spriteManager(spriteManager)     
{ 
}

entt::entity EntityFactory::createDragon()
{
    auto dragon = registry.create();
	registry.emplace<Position>(dragon, 5 * UNITS_PER_BLOCK, 5 * UNITS_PER_BLOCK);
	registry.emplace<RenderData>(dragon, RenderData(spriteManager.GetSpriteHandle("Dragon-Idle-1"), {2, 2}));
	registry.emplace<Collider>(dragon, 2 * UNITS_PER_BLOCK, UNITS_PER_BLOCK, 0, UNITS_PER_BLOCK);
    // Spikes on the back have full height and 6 pixels to the end of the sprite
	registry.emplace<DragonSpikeCollider>(dragon, 12 * UNITS_PER_PIXEL, 2 * UNITS_PER_BLOCK - 4 * UNITS_PER_PIXEL, 2 * UNITS_PER_BLOCK - 12 * UNITS_PER_PIXEL, 0);
	registry.emplace<WalkingActorComponent>(dragon);
	registry.emplace<DragonTag>(dragon);

    return dragon;
}