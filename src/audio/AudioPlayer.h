#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "raylib.h"


class AudioPlayer {
private:
	AudioPlayer() = default;

public:
	bool Init();
	void Close();
	void Update();

	Music& PlayMusic(const std::string& filepath);

	static AudioPlayer& Get();

private:
	std::vector<Music> musicList;
};
