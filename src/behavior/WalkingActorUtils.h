#pragma once

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../ecs/EntityFactory.h"
#include "../graphics/Animations.h"
#include "../utils/Debug.h"
#include "../utils/Input.h"
#include "Level.h"
#include "Physics.h"
#include "entt/entity/fwd.hpp"

bool shouldWalkingActorIgnoreCollisions(entt::registry &registry, const Position &pos, const WalkingActorComponent &actor);

bool isWalkingActorGrounded(entt::registry &registry, Position pos, const WalkingActorComponent &actor);
