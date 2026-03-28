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

bool Random::Bool() {
    return Range(1, 1000) <= 500;
}

bool Random::Chance(float chance) {
    return Range(1, 100'000) <= 100'000 * chance;
}

Random &Random::Get() {
    static Random random;
    return random;
}
