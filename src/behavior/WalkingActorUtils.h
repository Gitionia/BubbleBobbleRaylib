#pragma once

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../level/Level.h"
#include "../level/Physics.h"

bool shouldWalkingActorIgnoreCollisions(const Position &pos, const Collider& collider);

bool isWalkingActorGrounded(Position pos, const WalkingActorComponent &actor);

bool shouldWalkingEnemyGapJump(const Position &pos, int dir);
