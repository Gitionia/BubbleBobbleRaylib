#include "OnlineLeaderboard.h"

#if 0
#include <curl/curl.h>

static CURL *curl;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char *)contents, total_size);
    return total_size;
}

bool SendScoreToLeaderboard(const std::string &playerName, int score, int level_reached) {

    curl = curl_easy_init();

    CURLcode res;
    std::string response;

    // Can you leave out the select=* ?
    const std::string supabaseUrl =
        "https://mxknbbdufcrdzsoaugdc.supabase.co/rest/v1/leaderboard?select=*";
    const std::string apiKey = "sb_publishable_hSz_v651D6k2Y9JMah6XHw_Un4Jr1C0";

    if (playerName.size() != 3)
        return false;

    for (char c : playerName) {
        if (!std::isupper(c) && !std::isdigit(c))
            return false;
    }

    // JSON payload to insert a new entry
    // std::string jsonPayload = std::format(R"({{'player_name': '{}', 'score':
    // '{}'}})",
    //                                             playerName, score);

    const std::string jsonPayload = std::format(R"({{
            "player_name": "{}",
            "score": "{}",
            "level_reached": {}
            }})",
                                                playerName, score, level_reached);

    curl_easy_setopt(curl, CURLOPT_URL, supabaseUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("apikey: " + apiKey).c_str());
    headers =
        curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Prefer: return=representation");
    headers = curl_slist_append(headers, "Prefer: return=minimal");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the POST method and payload
    curl_easy_setopt(curl, CURLOPT_POST, 1L); // Enable POST
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                  << std::endl;
    } else {
        std::cout << "Success! Response:\n"
                  << response << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return true;
}

bool InitCurl() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    return curl != nullptr;
}

void CleanupCurl() {
    curl_global_cleanup();
}

#endif

#ifdef PLATFORM_DESKTOP
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

bool SendScoreToLeaderboard(const std::string &playerName, int score, int level_reached) {
    PRINT_INFO("Preparing sending to leaderboard: {}, score {}, level {}", playerName, score, level_reached);
    if (playerName.size() != 3)
        return false;

    for (char c : playerName) {
        if (!std::isupper(c) && !std::isdigit(c))
            return false;
    }

    try {
        httplib::Client cli("https://mxknbbdufcrdzsoaugdc.supabase.co/");

        const std::string apiKey = "sb_publishable_hSz_v651D6k2Y9JMah6XHw_Un4Jr1C0";
        const std::string jsonPayload = std::format(R"({{
            "player_name": "{}",
            "score": "{}",
            "level_reached": {}
            }})",
                                                    playerName, score, level_reached);

        httplib::Headers headers = {
            {"Authorization", ("Bearer " + apiKey).c_str()},
            {"apikey", apiKey},
            {"Prefer", "return=representation"},
            {"Prefer", "return=minimal"},
        };

        auto res = cli.Post("/rest/v1/leaderboard", headers, jsonPayload, "application/json");
        if (res.error() == httplib::Error::Success) {
            PRINT_INFO("Successfully send to leaderboard");
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

bool InitCurl() {
    return false;
}

void CleanupCurl() {
}

#else

#include <emscripten.h>
#include <emscripten/fetch.h>

#include <cstdio>
#include <cstring>

struct RequestData {
    std::string json;
};

static void on_success(emscripten_fetch_t *fetch) {

    PRINT_INFO("Request completed. HTTP status: {}", fetch->status);
    RequestData *request = static_cast<RequestData *>(fetch->userData);
    emscripten_fetch_close(fetch);
    delete request;
}

static void on_error(emscripten_fetch_t *fetch) {
    PRINT_ERROR("Request failed. HTTP status: {}", fetch->status);
    RequestData *request = static_cast<RequestData *>(fetch->userData);
    emscripten_fetch_close(fetch);
    delete request;
}

bool SendScoreToLeaderboard(const std::string &playerName, int score, int level_reached) {
    if (playerName.size() != 3)
        return false;

    for (char c : playerName) {
        if (!std::isupper(c) && !std::isdigit(c))
            return false;
    }

    const char *url = "https://mxknbbdufcrdzsoaugdc.supabase.co/rest/v1/leaderboard";
    const char *apiKey = "sb_publishable_hSz_v651D6k2Y9JMah6XHw_Un4Jr1C0";
    const char *bearer = "Bearer sb_publishable_hSz_v651D6k2Y9JMah6XHw_Un4Jr1C0";
    static const char *headers[] = {
        "Authorization",
        bearer,
        "apikey",
        apiKey,
        "Content-Type", "application/json",
        "Accept", "application/json",
        nullptr};

    std::string jsonPayload = std::format(R"({{
            "player_name": "{}",
            "score": "{}",
            "level_reached": {}
            }})",
                                          playerName, score, level_reached);

    RequestData* requestData = new RequestData();
    requestData->json = jsonPayload;

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

bool InitCurl() {
    return false;
}

void CleanupCurl() {
}

#endif