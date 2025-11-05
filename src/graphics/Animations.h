#pragma once
#include <memory>

#include "Sprites.h"


struct Animation {
	Animation& operator=(const Animation& other) {
		Sprites = other.Sprites;
		FrameCountPerSprite = other.FrameCountPerSprite;

		return *this;
	}

	std::vector<SpriteHandle> Sprites;
	// 	// How many frames the animation should take for each sprite
	int FrameCountPerSprite;
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
