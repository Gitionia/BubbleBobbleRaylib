#pragma once

#include "../ecs/Components.h"
#include "Level.h"
#include "entt/entity/fwd.hpp"

void setPhysicsColliderData(const LevelLayout &level);

int xPosToTileCoord(int x);
int yPosToTileCoord(int y);
bool collisionAt(int blockX, int blockY);

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
Vector2Int getBubbleRepelVelocity(const entt::registry& registry, const Collider &col, const Vector2Int &pos);


int calculateMovementToRoundedPosition(const Position &pos, const Collider &col, int dir);
template <typename Collider1, typename Collider2>
bool overlaps(const Position &pos1, const Collider1 &col1, const Position &pos2, const Collider2 &col2);
