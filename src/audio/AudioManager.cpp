#include "AudioManager.h"

#include "raylib.h"

bool AudioManager::Init() {
	InitAudioDevice();

	return IsAudioDeviceReady();
}

void AudioManager::Close() {
	CloseAudioDevice();
}
