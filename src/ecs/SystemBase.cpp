#include "SystemBase.h"


SystemBase::SystemBase(entt::registry &registry, const SpriteManager &spriteManager)
	: registry(registry), spriteManager(spriteManager)
{
}
