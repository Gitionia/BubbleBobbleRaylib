#include "Level.h"

void LevelLayout::SetTile(int index, bool value) {
	data[index] = value;
}

bool LevelLayout::Get(int x, int y) {
	int index = x + y * WIDTH;
	return data[index];
}
