#include "OnlineLeaderboard.h"

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