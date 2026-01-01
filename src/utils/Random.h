#pragma once

#include <random>

class Random {
  public:
    void Init();
    static Random& Get();
    
    int GetDirection();

  private:
    std::mt19937 rng {};

    friend class Application;
};
