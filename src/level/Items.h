#pragma once


enum class ItemType {
    Banana,
    Orange,
    Pear,
    Melon,
    Grapes,
    Pineapple,
    ElementCount
};

const char *GetItemSpriteName(ItemType item);
int GetItemPoints(ItemType item);
ItemType GetItemOfLevel(size_t level);
