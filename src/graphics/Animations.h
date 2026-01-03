#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Sprites.h"

struct Animation {
    Animation &operator=(const Animation &other) = default;

    std::vector<SpriteHandle> Sprites;
    
    std::string name;
    // 	// How many frames the animation should take for each sprite
    int FrameCountPerSprite;
};

void LoadAnimations();
Animation &GetAnimation(const std::string &name);

class Animator {
  public:
    Animator(const Animation *animation);

    void Update();
    bool IsFinished() const;
    void Reset();
    void SetNewAnimation(const Animation *animation);
    SpriteHandle GetSpriteHandle() const;
    const std::string& GetAnimationName() const;

  private:
    const Animation *animation;
    int currentSprite = 0;
    // How many frames the animation has been played
    int frame = 0;
};
