#pragma once
#include <entt/entity/registry.hpp>

#include "raylib.h"

class Debug {

public:
	static void DrawPoint(int x, int y, float radius, Color color);
	static void PrintNumberOfEntities();

private:
	void setRegistry(entt::registry& _registry);


	static Debug& Get();
	entt::registry* registry;

	friend class Application;
};
