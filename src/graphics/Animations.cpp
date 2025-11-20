#include "Animations.h"

#include <fstream>
#include "nlohmann/json.hpp"

#include "../utils/Log.h"

static std::unordered_map<std::string, Animation> animations;


void LoadAnimations() {
	const char* animationsFilePath = "res/sprites/Animations.json";
	std::ifstream f(animationsFilePath);
	if (f.fail()) {
		PRINT_ERROR("Animation information at {} is missing!", animationsFilePath);
		return;
	}

	nlohmann::json data = nlohmann::json::parse(f);

	auto animationJsonArray = data.find("animations").value();

	for (auto& animJson : animationJsonArray) {
		int framesPerSprite = animJson.find("FramesPerSprite").value();
		std::string animationBaseName = animJson.find("name").value();

		bool continueAdding = true;
		std::vector<SpriteHandle> animationFrames;
		while (continueAdding) {
			std::string animName = animationBaseName + "-" + std::to_string(animationFrames.size() + 1);
			SpriteHandle sprite = GetSpriteHandleChecked(animName);

			if (sprite == -1) {
				continueAdding = false;
			} else {
				animationFrames.push_back(sprite);
			}
		}

		animations.insert({ animationBaseName, { animationFrames, framesPerSprite } });
	}
}

Animation& GetAnimation(const std::string &name) {
	return animations.at(name);
}

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
