#pragma once

#include "entt/entt.hpp"

#include "EntityFactory.h"
#include "../audio/AudioManager.h"

#define SYSTEM_DEF(Type) public:\
	explicit Type(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager, const AudioManager& audio) :\
	SystemBase(registry, factory, spriteManager, audio) { }\
	private:


class SpriteManager;
class SystemBase {
public:
	SystemBase(entt::registry& registry, const EntityFactory& factory, const SpriteManager& spriteManager, const AudioManager& audio);
	virtual void Update() = 0;

protected:
	void Destroy(const entt::entity& e) const;

protected:
	entt::registry& registry;
	const EntityFactory& factory;
	const SpriteManager& spriteManager;
	const AudioManager& audio;
};