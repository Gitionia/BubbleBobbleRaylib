#pragma once

#include <string>

#include <fstream>

#include <nlohmann/json.hpp>

class LevelLayout {
public:
	LevelLayout() = default;

	void SetTile(int index, bool value);
	bool Get(int x, int y);

public:
	static constexpr int WIDTH = 28;
	static constexpr int HEIGHT = 26;
private:
	bool data[WIDTH * HEIGHT];
};

inline LevelLayout LoadLevel(const std::string& filepath) {
	std::ifstream f("res/levels/Level1.json");
	nlohmann::json data = nlohmann::json::parse(f);
	auto layers = data.find("layers").value();
	auto levelData = layers.at(0).find("data").value();

	if (levelData.size() != 26 * 28) throw "Invalid Size of leveldata";

	LevelLayout level{};

	for (int i = 0; i < levelData.size(); i++) {
		level.SetTile(i, (bool)(int)levelData.at(i));
	}

	return level;
}


