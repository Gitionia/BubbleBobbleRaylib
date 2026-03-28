#pragma once

class Random {
  public:
    void Init();
    static Random &Get();

    int GetDirection();
    int Range(int min, int max);
    int Range(int max);
    bool Bool();
    bool Chance(float chance);

  private:
    std::mt19937 rng{};

    friend class Application;
};
