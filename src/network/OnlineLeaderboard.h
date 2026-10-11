#pragma once

struct HttpRequestResult {
    enum SendResult {
        NONE = 0,
        SUCCESSFUL = 1,
        NOT_SUCCESSFUL = 2
    };

    bool IsCompleted = false;
    SendResult Result = NONE;

    void SetSuccessful(bool successful) {
        IsCompleted = true;
        Result = successful ? SUCCESSFUL : NOT_SUCCESSFUL;
    }

    static HttpRequestResult CreateSuccessful() {
        return {true, SUCCESSFUL};
    }

    static HttpRequestResult CreateNotSuccessful() {
        return {true, NOT_SUCCESSFUL};
    }

    static HttpRequestResult CreateNotCompleted() {
        return {false, NONE };
    }
};

bool InitCurl();
void CleanupCurl();
std::shared_ptr<HttpRequestResult> SendScoreToLeaderboard(const std::string &playerName, int score, int levelReached);
