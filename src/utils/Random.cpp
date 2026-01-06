#include "Random.h"

void Random::Init() {
    rng.seed(73);
}

int Random::GetDirection() {
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100);

    int dir = dist(rng) > 50 ? 1 : -1;
    return dir;
}

int Random::Range(int min, int max) {
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    
    return dist(rng);
}

int Random::Range(int max) {
    return Range(1, max);
}



Random &Random::Get() {
    static Random random;
    return random;
}
