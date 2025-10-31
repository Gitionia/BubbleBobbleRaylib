#pragma once

#include "entt/entt.hpp"
#include "../ecs/Components.h"
#include "Physics.h"

bool collidesWithWall(entt::registry& registry, const Position& position, const Collider& collider);
template <typename ColliderType>
bool collidesWithCollider(entt::registry& registry, const Position& position, const Collider& collider);
template <typename ColliderType>
bool collidesWithMultiCollider(entt::registry& registry, const Position& position, const Collider& collider);


int calculateMovementToRoundedPosition(const Position& pos, const Collider& col, int dir);
template <typename Collider1, typename Collider2>
bool overlaps(const Position& pos1, const Collider1& col1, const Position& pos2, const Collider2& col2);
