#pragma once

#include "Level.h"

enum class ItemType {
    Banana,
    Apple,
    Pear,
    Melon,
    Grapes,
    Pineapple,
    Soup,
    Meal,
    Shoe,
    Potion,
    ToyFlamingo,
    Door,
    Wand,
    ElementCount
};

const char *GetItemSpriteName(ItemType item);
int GetItemPoints(ItemType item);
ItemType GetItemOfLevel(size_t level);
const char *GetPointTextSpriteNameFromItemType(ItemType item);
ItemType GetItemTypeFromTile(LevelTileType levelTile);
