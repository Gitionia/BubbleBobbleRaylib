#include "Level.h"

#include <sstream>

#include "../utils/Log.h"

LevelTilemap::LevelTilemap()
	: data(SIZE) {
}

LevelTilemap::~LevelTilemap() {
}

LevelTileType LevelTilemap::Get(int x, int y, bool checked) const {
	if (checked && OutOfRange(x, y)) return LevelTileType::NONE;
	return data.at(idx(x, y));
}

bool LevelTilemap::IsEmpty(int x, int y) const {
	return Get(x, y) == LevelTileType::NONE;
}

void LevelTilemap::set(int x, int y, LevelTileType type) {
	data.at(idx(x, y)) = type;
}

void LevelTilemap::set(int index, LevelTileType type) {
	data.at(index) = type;
}

int LevelTilemap::idx(int x, int y) {
	return x + y * WIDTH;
}

int LevelTilemap::OutOfRange(int x, int y) {
	return idx(x, y) < 0 || idx(x, y) >= SIZE;
}

void LevelLayout::SetColors(Color right, Color bottem) {
	tileShadeColorRight = right;
	tileShadeColorBottem = bottem;
}

Color LevelLayout::GetShadeColorRight() const {
	return tileShadeColorRight;
}

Color LevelLayout::GetShadeColorBottem() const {
	return tileShadeColorBottem;
}

const LevelTilemap &LevelLayout::GetTiles() const {
	return tiles;
}

const LevelTilemap &LevelLayout::GetAirflow() const {
	return airflow;
}

unsigned int parseColorChannel(const std::string &str) {
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return x;
}

Color parseColors(const std::string &s) {
	unsigned char a = parseColorChannel(s.substr(1, 2));
	unsigned char r = parseColorChannel(s.substr(3, 2));
	unsigned char g = parseColorChannel(s.substr(5, 2));
	unsigned char b = parseColorChannel(s.substr(7, 2));

	return {r, g, b, a};
}

LevelLayout LevelLayout::LoadLevel(const std::string &filepath) {
	LevelLayout level{};

	std::ifstream f(filepath);
	if (f.fail()) {
		PRINT_ERROR("Level data at {} is missing!", filepath.c_str());
		return level;
	}

	nlohmann::json data = nlohmann::json::parse(f);
	auto layers = data.find("layers").value();

	for (auto &layer: layers) {
		if (layer.find("name").value() == "Tiles") {
			auto levelData = layer.find("data").value();

			auto colors = layer.find("properties").value();
			std::string colorRight;
			std::string colorBottem;

			for (auto &color: colors) {
				if (std::string(color.find("name").value()) == "ShadeBottom") {
					colorBottem = color.find("value").value();
				}
				if (std::string(color.find("name").value()) == "ShadeRight") {
					colorRight = color.find("value").value();
				}
			}

			if (colorRight.empty() || colorBottem.empty()) {
				PRINT_INFO("Shade Colors of Level at {} could not be loaded. Maybe missing?", filepath.c_str());
			} else {
				level.SetColors(parseColors(colorRight), parseColors(colorBottem));
			}


			if (levelData.size() != 26 * 28)
				PRINT_ERROR("Level at {} contains leveldata with invalid length on layer 'Tiles'", filepath.c_str());

			for (int i = 0; i < levelData.size(); i++) {
				level.tiles.set(i, levelData.at(i));
			}
		}
		else if (layer.find("name").value() == "Airflow") {
			auto levelData = layer.find("data").value();

			if (levelData.size() != 26 * 28)
				PRINT_ERROR("Level at {} contains leveldata with invalid length on layer 'Airflow'", filepath.c_str());

			for (int i = 0; i < levelData.size(); i++) {
				level.airflow.set(i, levelData.at(i));
			}
		}
	}

	return level;
}
