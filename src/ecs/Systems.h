#pragma once

#include <entt/entt.hpp>

#define ECS_SYSTEM(type)\
public:\
	void setRegistry(entt::registry& registry);\
	void update();\
\
private:\
	entt::registry& registry;


void initECSSystems(entt::registry& registry);
void updateECSSystems();
