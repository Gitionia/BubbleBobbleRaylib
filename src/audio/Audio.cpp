#include "Audio.h"
#include "raylib.h"

static std::vector<Music> s_musicList;
static std::unordered_map<std::string, Music> s_musicMap;
static std::unordered_map<std::string, Sound> s_soundMap;

bool InitAudio() {
    InitAudioDevice();
    return IsAudioDeviceReady();
}

void LoadAudio() {
    const char *animationsFilePath = "res/sounds/Audio.json";
    std::ifstream f(animationsFilePath);
    if (f.fail()) {
        PRINT_CRITICAL("Audio.json at {} is missing!", animationsFilePath);
        return;
    }

    nlohmann::json data = nlohmann::json::parse(f);

    for (int i = 0; i < 2; i++) {
        const char *jsonArrayFieldName = i == 0 ? "sound" : "music";
        auto jsonArray = data.find(jsonArrayFieldName).value();

        for (auto &jsonElement : jsonArray) {
            std::string name = jsonElement.find("name").value();
            std::string path = jsonElement.find("path").value();

            if (i == 0) {
                Sound sound = LoadSound(path.c_str());
                s_soundMap.insert({name, sound});
            } else {
                Music music = LoadMusicStream(path.c_str());
                s_musicList.push_back(music);
                s_musicMap.insert({name, music});
            }
        }
    }
}


void CleanUpAudio() {
    for (auto &sound : s_soundMap) {
        UnloadSound(sound.second);
    }
    for (auto &music : s_musicMap) {
        UnloadMusicStream(music.second);
    }

    CloseAudioDevice();
}

void UpdateAudio() {
    for (const auto &music : s_musicList) {
        UpdateMusicStream(music);
    }
}

const Sound &GetSound(const std::string &name) {
    return s_soundMap.at(name);
}
const Music &GetMusic(const std::string &name) {
    return s_musicMap.at(name);
}
