#pragma once

#include "entt/entt.hpp"
#include "../ecs/Components.h"
#include "Physics.h"

bool collidesWithWall(entt::registry& registry, const Position& position, const Collider& collider);
bool overlaps(const Position& pos1, const Collider& col1, const Position& pos2, const Collider& col2);
