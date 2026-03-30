#pragma once

struct Recording {
  public:
    Recording();

    void SaveToFile(const std::string &filepath);
    void ReadFromFile(const std::string &filepath);

    static constexpr int KEY_COUNT = 512;
    static constexpr int BUFFER_SIZE = KEY_COUNT * 60 * 180;
    std::vector<uint8_t> data{};

    uint8_t current[KEY_COUNT]{};
    uint8_t previous[KEY_COUNT]{};

    int frameIndex = 0;
};

class InputSimulator {
  public:
    enum Mode {
        NO_RECORD,
        RECORD,
        REPLAY
    };

    InputSimulator(Mode mode, std::string filepath = "");
    void SaveRecording();

    void Update();

    bool IsKeyDown(int key);
    bool IsKeyPressed(int key);

  private:
    Recording recording;
    Mode mode;
    std::string filepath;

    bool exceededRecording = false;
};
