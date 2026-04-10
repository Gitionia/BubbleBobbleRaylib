#include "Dragons.h"

const char *GetDragonAnimation(DragonAnimationType animation, DragonColor color) {
    if (color == DRAGON_GREEN) {

        switch (animation) {
        case DragonAnimationType::IDLE:
            return "Dragon-Idle";
        case DragonAnimationType::WALKING:
            return "Dragon-Walking";
        case DragonAnimationType::SHOOTING:
            return "Dragon-Shooting";
        case DragonAnimationType::JUMPING:
            return "Dragon-Jumping";
        case DragonAnimationType::FALLING:
            return "Dragon-Falling";
        case DragonAnimationType::HIT:
            return "Dragon-Hit";
        case DragonAnimationType::HIT_STARE:
            return "Dragon-HitStare";
        case DragonAnimationType::RESPAWN:
            return "Dragon-Respawn";
        default:
            DBG_CHECK(false, "Tried to get name of invalid dragon animation type");
            return GetDragonAnimation(DragonAnimationType::IDLE, color);
        }
    }

    else {
        DBG_CHECK(false, "Blue Dragon animations not implemented");
        return GetDragonAnimation(animation, DRAGON_GREEN);
    }
}