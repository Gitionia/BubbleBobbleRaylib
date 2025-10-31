#pragma once

#include "entt/entt.hpp"

#define SYSTEM_DEF(Type) public:\
	explicit Type(entt::registry& registry, const SpriteManager& spriteManager) :\
	SystemBase(registry, spriteManager) { }\
	private:


class SpriteManager;
class SystemBase {
public:
	SystemBase(entt::registry& registry, const SpriteManager& spriteManager);
	virtual void Update() = 0;

protected:
	void Destroy(const entt::entity& e) const;

protected:
	entt::registry& registry;
	const SpriteManager& spriteManager;
};