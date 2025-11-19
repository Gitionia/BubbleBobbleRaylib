#include "AudioPlayer.h"

#include "raylib.h"

bool AudioPlayer::Init() {
	InitAudioDevice();

	return IsAudioDeviceReady();
}

void AudioPlayer::Close() {
	CloseAudioDevice();
}
