#pragma once

#include "../ecs/Components.h"
#include "Level.h"
#include "entt/entity/fwd.hpp"
#include "entt/entt.hpp"

#include "../utils/Utilities.h"
#include <optional>

void setPhysicsColliderData(const LevelLayout &level);

bool collidesWithWall(entt::registry &registry, const Position &position, const Collider &collider);
template <typename ColliderType>
bool collidesWithCollider(entt::registry &registry, const Position &position, const Collider &collider);
template <typename ColliderType>
bool collidesWithMultiCollider(entt::registry &registry, const Position &position, const Collider &collider);

bool collidesWithDragonSpikes(entt::registry &registry, const Position &position, const Collider &collider);
bool collidesWithJumpableBubble(entt::registry &registry, const Position &position, const Collider &collider);
std::optional<entt::entity> getCollidingShootingBubble(entt::registry &registry, const Position &position, const Collider &collider);

bool collidesWithEnemy(entt::registry &registry, const Position &position, const Collider &collider);

Vector2Int getAirflowDirection(const Collider &col, const Vector2Int &pos);

int calculateMovementToRoundedPosition(const Position &pos, const Collider &col, int dir);
template <typename Collider1, typename Collider2>
bool overlaps(const Position &pos1, const Collider1 &col1, const Position &pos2, const Collider2 &col2);
