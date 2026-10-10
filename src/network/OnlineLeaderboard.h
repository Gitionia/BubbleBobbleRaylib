#pragma once

#include <curl/curl.h>

bool InitCurl();
void CleanupCurl();
bool SendScoreToLeaderboard(const std::string &playerName, int score, int levelReached);
