#pragma once

enum class DragonAnimationType {
    IDLE,
    WALKING,
    SHOOTING,
    JUMPING,
    FALLING,
    HIT,
    HIT_STARE,
    RESPAWN,
};

enum DragonColor {
    DRAGON_GREEN = 0,
    DRAGON_BLUE = 1
};

const char *GetDragonAnimation(DragonAnimationType animation, DragonColor color);