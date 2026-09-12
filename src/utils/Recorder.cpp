#include "Recorder.h"
#include "Log.h"
#include "raylib.h"
#include <cstdint>

void Recording::Init() {
    data.reserve(BUFFER_SIZE);
}

void Recording::SaveToFile(const std::string &filepath) {
    std::ofstream file(filepath, std::ios::binary);

    if (!file) {
        PRINT_WARN("Could not save input recording to {}", filepath);
        return;
    }

    file.write(reinterpret_cast<const char *>(data.data()), data.size());

    PRINT_INFO("Saved recording to {}", filepath);
    
    file.close();
}

void Recording::ReadFromFile(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);

    if (!file) {
        PRINT_ERROR("Could not read input recording from {}", filepath);
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size <= 0) {
        PRINT_ERROR("Input recording at {} has no elements", filepath);
        return;
    }

    data.resize(size);
    file.read(reinterpret_cast<char *>(data.data()), size);

    PRINT_INFO("Read recording from {}", filepath);

    file.close();

    frameIndex = 0;
}

void InputSimulator::Init(Mode mode, std::string filepath) {
    // We don't do anything if we set NO_RECORD
    if (mode == RECORD || mode == REPLAY) {
        recording.Init();
    }
    if (mode == REPLAY) {
        recording.ReadFromFile(filepath);
    }

    this->mode = mode;
    this->filepath = filepath;
}

void InputSimulator::SaveRecording() {
    if (mode == RECORD) {
        recording.SaveToFile(filepath);
    }
}

void InputSimulator::Update() {
    if (mode == NO_RECORD || exceededRecording) {
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
        for (int key = 0; key < Recording::KEY_COUNT; key++) {
            size_t bufferIndex = key + recording.frameIndex * Recording::KEY_COUNT;
            if (bufferIndex >= recording.data.size()) {
                PRINT_WARN("Finished using input recording! Switching to live input.");
                exceededRecording = true;
                break;
            }

            recording.current[key] = recording.data.at(bufferIndex);
        }
    }

    recording.frameIndex++;
}

bool InputSimulator::IsKeyDown(int key) {
    if (mode == NO_RECORD || exceededRecording) {
        return ::IsKeyDown(key);

    } else {
        return recording.current[key];
    }
}

bool InputSimulator::IsKeyPressed(int key) {
    if (mode == NO_RECORD || exceededRecording) {
        return ::IsKeyPressed(key);

    } else {
        return recording.current[key] && !recording.previous[key];
    }
}
