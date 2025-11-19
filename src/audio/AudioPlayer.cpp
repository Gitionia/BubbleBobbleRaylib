#include "AudioPlayer.h"

#include "raylib.h"

bool AudioPlayer::Init() {
	InitAudioDevice();

	return IsAudioDeviceReady();
}

void AudioPlayer::Close() {
	CloseAudioDevice();
}

void AudioPlayer::Update() {
	for (auto& music : musicList) {
		UpdateMusicStream(music);
	}
}

Music& AudioPlayer::PlayMusic(const std::string &filepath) {
	musicList.push_back(LoadMusicStream(filepath.c_str()));
	Music& music = musicList.back();
	PlayMusicStream(music);

	return music;
}

AudioPlayer &AudioPlayer::Get() {
	static AudioPlayer audio;
	return audio;
}
