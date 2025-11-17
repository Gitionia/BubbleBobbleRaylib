#include "Level.h"

void LevelLayout::SetTile(int index, bool value) {
	data[index] = value;
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

bool LevelLayout::Get(int x, int y) const {
	int index = x + y * WIDTH;
	return data[index];
}

bool LevelLayout::GetWithBoundaryCheck(int x, int y) const {
	int index = x + y * WIDTH;
	if (index < 0 || index > WIDTH * HEIGHT) {
		return false;
	}

	return data[index];
}

unsigned int parseColorChannel(const std::string & str) {
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return x;
}

Color parseColors(const std::string& s) {
	unsigned char a = parseColorChannel(s.substr(1, 2));
	unsigned char r = parseColorChannel(s.substr(3, 2));
	unsigned char g = parseColorChannel(s.substr(5, 2));
	unsigned char b = parseColorChannel(s.substr(7, 2));

	printf("%hhu, %hhu, %hhu, %hhu", r, g, b, a);

	return {r, g, b, a};
}

LevelLayout LoadLevel(const std::string &filepath) {
	LevelLayout level{};


	std::ifstream f(filepath);
	nlohmann::json data = nlohmann::json::parse(f);
	auto layers = data.find("layers").value();
	auto levelData = layers.at(0).find("data").value();

	auto colors = layers.at(0).find("properties").value();
	std::string colorRight;
	std::string colorBottem;

	for (auto& color : colors) {
		if (std::string(color.find("name").value()) == "ShadeBottom") {
			colorBottem = color.find("value").value();
		}
		if (std::string(color.find("name").value()) == "ShadeRight") {
			colorRight = color.find("value").value();
		}
	}

	if (colorRight.empty() || colorBottem.empty()) {
		printf("Shade Colors of Level at %s could not be loaded. Maybe missing?", filepath.c_str());
	} else {
		level.SetColors(parseColors(colorRight), parseColors(colorBottem));
	}


	if (levelData.size() != 26 * 28) throw "Invalid Size of leveldata";


	for (int i = 0; i < levelData.size(); i++) {
		level.SetTile(i, (bool)(int)levelData.at(i));
	}

	return level;
}
