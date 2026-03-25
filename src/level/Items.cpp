#include "Items.h"

const char *GetItemSpriteName(ItemType item) {
    switch (item) {

    case ItemType::Banana:
        return "Item-Banana";
    case ItemType::Apple:
        return "Item-Apple";
    case ItemType::Pear:
        return "Item-Pear";
    case ItemType::Melon:
        return "Item-Melon";
    case ItemType::Grapes:
        return "Item-Grapes";
    case ItemType::Pineapple:
        return "Item-Pineapple";
    case ItemType::ElementCount:
        PRINT_ERROR("Invalid enum value for ItemType");
        DBG_ASSERT(false);
        return GetItemSpriteName(ItemType::Banana);
    }
}

int GetItemPoints(ItemType item) {
    switch (item) {

    case ItemType::Banana:
        return 500;
    case ItemType::Apple:
        return 1000;
    case ItemType::Pear:
        return 2000;
    case ItemType::Melon:
        return 3000;
    case ItemType::Grapes:
        return 6000;
    case ItemType::Pineapple:
        return 8000;
    case ItemType::ElementCount:
        PRINT_ERROR("Invalid enum value for ItemType");
        DBG_ASSERT(false);
        return GetItemPoints(ItemType::Banana);
    }
}

ItemType GetItemOfLevel(size_t level) {
    if (level >= (int)ItemType::ElementCount)
        return ItemType::Pineapple;
    else
        return (ItemType)level;
}

const char *GetPointTextSpriteNameFromItemType(ItemType item) {
    switch (item) {

    case ItemType::Banana:
        return "Points-500";
    case ItemType::Apple:
        return "Points-1000";
    case ItemType::Pear:
        return "Points-2000";
    case ItemType::Melon:
        return "Points-3000";
    case ItemType::Grapes:
        return "Points-6000";
    case ItemType::Pineapple:
        return "Points-8000";
    case ItemType::ElementCount:
        PRINT_ERROR("Invalid enum value for ItemType");
        DBG_ASSERT(false);
        return GetPointTextSpriteNameFromItemType(ItemType::Banana);
    }
}
