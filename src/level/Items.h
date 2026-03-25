#pragma once


enum class ItemType {
    Banana,
    Apple,
    Pear,
    Melon,
    Grapes,
    Pineapple,
    ElementCount
};

const char *GetItemSpriteName(ItemType item);
int GetItemPoints(ItemType item);
ItemType GetItemOfLevel(size_t level);
const char *GetPointTextSpriteNameFromItemType(ItemType item);
