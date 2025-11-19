#pragma once

#include "entt/entt.hpp"

#include "EntityFactory.h"

#define SYSTEM_DEF(Type) public:\
	explicit Type(entt::registry& registry, const EntityFactory& factory) :\
	SystemBase(registry, factory) { }\
	private:


class SpriteManager;
class SystemBase {
public:
	SystemBase(entt::registry& registry, const EntityFactory& factory);
	virtual void Init() {}
	virtual void Update() = 0;

protected:
	void Destroy(const entt::entity& e) const;

protected:
	entt::registry& registry;
	const EntityFactory& factory;
};