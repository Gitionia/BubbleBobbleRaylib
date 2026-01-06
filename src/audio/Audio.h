#pragma once

bool InitAudio();
void UnloadAllAudio();
void UpdateAudio();

Music &PlayMusic(const std::string &filepath);
