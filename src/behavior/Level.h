#pragma once

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


