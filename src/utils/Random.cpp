#include "Random.h"
#include <random>

void Random::Init() {
    rng.seed(73);
}

int Random::GetDirection() {
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100);

    int dir = dist(rng) > 50 ? 1 : -1;
    return dir;
}

Random &Random::Get() {
    static Random random;
    return random;
}
