#include "Audio.h"

#include "raylib.h"

static std::vector<Music> musicList;

bool InitAudio() {
	InitAudioDevice();
	return IsAudioDeviceReady();
}

void UnloadAllAudio() {
	for (auto& music : musicList) {
		UnloadMusicStream(music);
	}
}

void UpdateAudio() {
	for (const auto& music : musicList) {
		UpdateMusicStream(music);
	}
}

Music& PlayMusic(const std::string &filepath) {
	musicList.push_back(LoadMusicStream(filepath.c_str()));
	Music& music = musicList.back();
	PlayMusicStream(music);

	return music;
}

