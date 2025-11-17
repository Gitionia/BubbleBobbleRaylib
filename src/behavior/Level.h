#pragma once

#include <string>

#include <fstream>

#include <nlohmann/json.hpp>

#include "raylib.h"

class LevelLayout {
public:
	LevelLayout() = default;

	void SetTile(int index, bool value);
	void SetColors(Color right, Color bottem);
	Color GetShadeColorRight() const;
	Color GetShadeColorBottem() const;
	bool Get(int x, int y) const;
	bool GetWithBoundaryCheck(int x, int y) const;

public:
	static constexpr int WIDTH = 28;
	static constexpr int HEIGHT = 26;
private:
	bool data[WIDTH * HEIGHT];
	Color tileShadeColorRight;
	Color tileShadeColorBottem;
};

LevelLayout LoadLevel(const std::string& filepath);


