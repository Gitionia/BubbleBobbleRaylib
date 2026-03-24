#pragma once

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "Level.h"
#include "Physics.h"

bool shouldWalkingActorIgnoreCollisions(entt::registry &registry, const Position &pos, const Collider& collider);

bool isWalkingActorGrounded(entt::registry &registry, Position pos, const WalkingActorComponent &actor);
