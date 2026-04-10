#include "Dragons.h"

const char *GetDragonAnimation(DragonAnimationType animation, DragonColor color) {
    if (color == DRAGON_GREEN) {

        switch (animation) {
        case DragonAnimationType::IDLE:
            return "DragonGreen-Idle";
        case DragonAnimationType::WALKING:
            return "DragonGreen-Walking";
        case DragonAnimationType::SHOOTING:
            return "DragonGreen-Shooting";
        case DragonAnimationType::JUMPING:
            return "DragonGreen-Jumping";
        case DragonAnimationType::FALLING:
            return "DragonGreen-Falling";
        case DragonAnimationType::HIT:
            return "DragonGreen-Hit";
        case DragonAnimationType::HIT_STARE:
            return "DragonGreen-HitStare";
        case DragonAnimationType::RESPAWN:
            return "DragonGreen-Respawn";
        default:
            DBG_CHECK(false, "Tried to get name of invalid dragon animation type");
            return GetDragonAnimation(DragonAnimationType::IDLE, color);
        }
    }

    else {
        switch (animation) {
        case DragonAnimationType::IDLE:
            return "DragonBlue-Idle";
        case DragonAnimationType::WALKING:
            return "DragonBlue-Walking";
        case DragonAnimationType::SHOOTING:
            return "DragonBlue-Shooting";
        case DragonAnimationType::JUMPING:
            return "DragonBlue-Jumping";
        case DragonAnimationType::FALLING:
            return "DragonBlue-Falling";
        case DragonAnimationType::HIT:
            return "DragonBlue-Hit";
        case DragonAnimationType::HIT_STARE:
            return "DragonBlue-HitStare";
        case DragonAnimationType::RESPAWN:
            return "DragonBlue-Respawn";
        default:
            DBG_CHECK(false, "Tried to get name of invalid dragon animation type");
            return GetDragonAnimation(DragonAnimationType::IDLE, color);
        }
    }
}