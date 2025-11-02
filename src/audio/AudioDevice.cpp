#include "AudioDevice.h"

#include "raylib.h"

bool AudioDevice::Init() {
	InitAudioDevice();

	return IsAudioDeviceReady();
}

void AudioDevice::Close() {
	CloseAudioDevice();
}
