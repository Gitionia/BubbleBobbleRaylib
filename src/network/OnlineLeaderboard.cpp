#include "OnlineLeaderboard.h"

static bool SendPostRequest(std::string json);

bool SendScoreToLeaderboard(const std::string &playerName, int score, int level_reached) {
    if (playerName.size() != 3)
        return false;

    for (char c : playerName) {
        if (!std::isupper(c) && !std::isdigit(c))
            return false;
    }
    const std::string jsonPayload = std::format(R"({{
        "player_name": "{}",
        "score": "{}",
        "level_reached": {}
        }})",
                                                playerName, score, level_reached);

    return SendPostRequest(jsonPayload);
}

#define API_KEY "sb_publishable_hSz_v651D6k2Y9JMah6XHw_Un4Jr1C0"
#define BEARER "Bearer" API_KEY

#ifdef PLATFORM_DESKTOP

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
static bool SendPostRequest(std::string json) {

    try {
        httplib::Client cli("https://mxknbbdufcrdzsoaugdc.supabase.co/");

        httplib::Headers headers = {
            {"Authorization", BEARER},
            {"apikey", API_KEY},
            {"Prefer", "return=representation"},
            {"Prefer", "return=minimal"},
        };

        auto res = cli.Post("/rest/v1/leaderboard", headers, json, "application/json");
        if (res.error() == httplib::Error::Success) {
            PRINT_INFO("Successfully sent to leaderboard");
            return true;

        } else if (res.error() == httplib::Error::Unknown) {
            PRINT_ERROR("Could not send to leaderboard, due to unknown reason");
            return false;
        } else {
            PRINT_ERROR("Could not send to leaderboard!");
            return false;
        }

    } catch (std::exception e) {
        PRINT_ERROR("Could not send to leaderboard. Exception: {}", e.what());
        return false;
    }
}

#else

#include <emscripten.h>
#include <emscripten/fetch.h>

struct RequestData {
    std::string json;
};

static void on_success(emscripten_fetch_t *fetch) {

    PRINT_INFO("Successfully sent to leaderboard. HTTP status: {}", fetch->status);
    RequestData *request = static_cast<RequestData *>(fetch->userData);
    emscripten_fetch_close(fetch);
    delete request;
}

static void on_error(emscripten_fetch_t *fetch) {
    PRINT_ERROR("Could not send to leaderboard. HTTP status: {}", fetch->status);
    RequestData *request = static_cast<RequestData *>(fetch->userData);
    emscripten_fetch_close(fetch);
    delete request;
}

static bool SendPostRequest(std::string json) {

    const char *url = "https://mxknbbdufcrdzsoaugdc.supabase.co/rest/v1/leaderboard";
    static const char *headers[] = {
        "Authorization",
        BEARER,
        "apikey",
        API_KEY,
        "Content-Type", "application/json",
        "Accept", "application/json",
        nullptr};

    RequestData *requestData = new RequestData();
    requestData->json = json;

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    std::strcpy(attr.requestMethod, "POST");

    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.requestData = requestData->json.c_str();
    attr.requestDataSize = requestData->json.size();
    attr.requestHeaders = headers;

    attr.userData = requestData;
    attr.onsuccess = on_success;
    attr.onerror = on_error;

    emscripten_fetch(&attr, url);

    return 0;
}

#endif