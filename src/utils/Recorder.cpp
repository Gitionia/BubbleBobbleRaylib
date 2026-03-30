#include "Recorder.h"
#include "raylib.h"

Recording::Recording() {
    data.reserve(BUFFER_SIZE);
}

void Recording::SaveToFile(const std::string &filepath) {
    std::ofstream file(filepath, std::ios::binary);

    file.write(reinterpret_cast<const char *>(data.data()), data.size());
}

void Recording::ReadFromFile(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::binary);
    data = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    frameIndex = 0;
}

InputSimulator::InputSimulator(Mode mode, std::string filepath)
    : mode(mode), filepath(filepath) {
}

void InputSimulator::SaveRecording() {
    recording.SaveToFile(filepath);
}

void InputSimulator::Update() {
    if (mode == NO_RECORD) {
        return;
    }

    std::memcpy(recording.previous, recording.current, Recording::KEY_COUNT);
    if (mode == RECORD) {
        for (int key = 0; key < Recording::KEY_COUNT; key++) {
            bool value = ::IsKeyDown(key);

            recording.current[key] = value;
            recording.data.push_back(value);
        }

    } else if (mode == REPLAY) {
        for (int i = 0; i < Recording::KEY_COUNT; i++) {
            recording.current[i] = i + recording.frameIndex * Recording::KEY_COUNT;
        }
    }

    recording.frameIndex++;
}

bool InputSimulator::IsKeyDown(int key) {
    if (mode == NO_RECORD) {
        return IsKeyDown(key);

    } else {
        return recording.current[key];
    }
}

bool InputSimulator::IsKeyPressed(int key) {
    if (mode == NO_RECORD) {
        return IsKeyPressed(key);

    } else {
        return recording.current[key] && !recording.previous[key];
    }
}
