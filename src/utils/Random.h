#pragma once

class Random {
  public:
    void Init();
    static Random &Get();

    int GetDirection();
    // Gets a random int from closed interval [min,max]
    int Range(int min, int max);
    // Gets a random int from closed interval [1,max]
    int Range(int max);
    bool Bool();
    bool Chance(float chance);

  private:
    std::mt19937 rng{};

    friend class Application;
};
