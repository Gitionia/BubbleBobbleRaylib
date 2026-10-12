#include "OnlineLeaderboard.h"

static std::shared_ptr<HttpRequestResult> SendPostRequest(std::string json);

std::shared_ptr<HttpRequestResult> SendScoreToLeaderboard(const std::string &playerName, int score, int level_reached) {
    if (playerName.size() != 3)
        return std::make_shared<HttpRequestResult>(HttpRequestResult::CreateNotSuccessful());

    for (char c : playerName) {
        if (!std::isupper(c) && !std::isdigit(c)) {
            return std::make_shared<HttpRequestResult>(HttpRequestResult::CreateNotSuccessful());
        }
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
#define BEARER "Bearer " API_KEY

#ifdef PLATFORM_DESKTOP

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../../deps/cpp-httplib/httplib.h"
static std::shared_ptr<HttpRequestResult> SendPostRequest(std::string json) {

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
            return std::make_shared<HttpRequestResult>(HttpRequestResult::CreateSuccessful());

        } else if (res.error() == httplib::Error::Unknown) {
            PRINT_ERROR("Could not send to leaderboard, due to unknown reason");
            return std::make_shared<HttpRequestResult>(HttpRequestResult::CreateNotSuccessful());

        } else {
            PRINT_ERROR("Could not send to leaderboard!");
            return std::make_shared<HttpRequestResult>(HttpRequestResult::CreateNotSuccessful());
        }

    } catch (std::exception e) {
        PRINT_ERROR("Could not send to leaderboard. Exception: {}", e.what());
        return std::make_shared<HttpRequestResult>(HttpRequestResult::CreateNotSuccessful());
    }
}

#else

#include <emscripten.h>
#include <emscripten/fetch.h>

struct RequestData {
    std::string json;
    std::shared_ptr<HttpRequestResult> result;
};

static RequestData s_requestData = {"", nullptr};

static void on_success(emscripten_fetch_t *fetch) {

    s_requestData.result->SetSuccessful(true);
    s_requestData.result = nullptr;
    s_requestData.json = "";

    PRINT_INFO("Successfully sent to leaderboard. HTTP status: {}", fetch->status);
    emscripten_fetch_close(fetch);
}

static void on_error(emscripten_fetch_t *fetch) {

    s_requestData.result->SetSuccessful(false);
    s_requestData.result = nullptr;
    s_requestData.json = "";

    PRINT_ERROR("Could not send to leaderboard. HTTP status: {}", fetch->status);
    emscripten_fetch_close(fetch);
}

static std::shared_ptr<HttpRequestResult> SendPostRequest(std::string json) {

    const char *url = "https://mxknbbdufcrdzsoaugdc.supabase.co/rest/v1/leaderboard";
    static const char *headers[] = {
        "Authorization",
        BEARER,
        "apikey",
        API_KEY,
        "Content-Type", "application/json",
        "Accept", "application/json",
        nullptr};

    s_requestData.json = json;
    s_requestData.result = std::make_shared<HttpRequestResult>(HttpRequestResult::CreateNotCompleted());

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    std::strcpy(attr.requestMethod, "POST");

    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.requestData = s_requestData.json.c_str();
    attr.requestDataSize = s_requestData.json.size();
    attr.requestHeaders = headers;

    attr.onsuccess = on_success;
    attr.onerror = on_error;

    emscripten_fetch(&attr, url);

    return s_requestData.result;
}

#endif