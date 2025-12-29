#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "raylib.h"

bool InitAudio();
void UnloadAllAudio();
void UpdateAudio();

Music &PlayMusic(const std::string &filepath);
