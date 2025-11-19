#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "Sprites.h"



struct Animation {
	Animation& operator=(const Animation& other) = default;

	std::vector<SpriteHandle> Sprites;
	// 	// How many frames the animation should take for each sprite
	int FrameCountPerSprite;
};

class Animations {
public:
	static Animations& Get();
	void LoadAnimations();
	Animation& GetAnimation(const std::string& name);

private:
	std::unordered_map<std::string, Animation> animations;
};

class Animator {
public:
	Animator(const Animation* animation);

	bool Update();
	bool IsFinished() const;
	void Reset();
	void SetNewAnimation(const Animation* animation);
	SpriteHandle GetSpriteHandle() const;

private:
	const Animation* animation;
	int currentSprite = 0;
	// How many frames the animation has been played
	int frame = 0;
};
