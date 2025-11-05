#include "Animations.h"


Animator::Animator(const Animation* animation)
	: animation(animation) { }

bool Animator::Update() {
	frame++;
	currentSprite = frame / animation->FrameCountPerSprite;

	return IsFinished();
}

bool Animator::IsFinished() const {
	return currentSprite >= animation->Sprites.size();
}

void Animator::Reset() {
	frame = 0;
	currentSprite = 0;
}

void Animator::SetNewAnimation(const Animation* animation) {
	Reset();
	this->animation = animation;
}

SpriteHandle Animator::GetSpriteHandle() const {
	return animation->Sprites.at(currentSprite);
}
