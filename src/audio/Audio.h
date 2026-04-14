#pragma once

bool InitAudio();
void LoadAudio();
void CleanUpAudio();
void UpdateAudio();

void StopAllMusic();

const Sound& GetSound(const std::string& name);
const Music& GetMusic(const std::string& name);
