#include "Items.h"
#include "Level.h"

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
    case ItemType::Shoe:
        return "Item-Shoe";
    case ItemType::ToyFlamingo:
        return "Item-ToyFlamingo";
    case ItemType::Book:
        return "Item-Book";
    case ItemType::Soup:
        return "Item-Soup";
    case ItemType::Meal:
        return "Item-Meal";
    case ItemType::Potion:
        DBG_CHECK(false, "Multiple potions not implemented yet!");
        return "Item-Potion-Green";
    case ItemType::Door:
        return "ITem-Door";
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
    case ItemType::Shoe:
    case ItemType::Potion:
        return 1000;
    case ItemType::Soup:
    case ItemType::Meal:
        return 2000;
    case ItemType::ToyFlamingo:
    case ItemType::Door:
    case ItemType::Book:
        return 0;
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
    case ItemType::Shoe:
    case ItemType::Potion:
        return "Points-1000";
    case ItemType::Soup:
    case ItemType::Meal:
        return "Points-2000";
    case ItemType::ToyFlamingo:
    case ItemType::Door:
    case ItemType::Book:
        DBG_CHECK(false, "Item has no points and doesn't spawn a point text");
        return GetPointTextSpriteNameFromItemType(ItemType::Banana);
    case ItemType::ElementCount:
        PRINT_ERROR("Invalid enum value for ItemType");
        DBG_ASSERT(false);
        return GetPointTextSpriteNameFromItemType(ItemType::Banana);
    }
}
ItemType GetItemTypeFromTile(LevelTileType levelTile) {
    switch (levelTile) {

    case LevelTileType::ITEM_SOUP:
        return ItemType::Soup;
    case LevelTileType::ITEM_MEAL:
        return ItemType::Meal;
    case LevelTileType::ITEM_SHOE:
        return ItemType::Shoe;
    case LevelTileType::ITEM_POTION:
        return ItemType::Potion;
    case LevelTileType::ITEM_DOOR:
        return ItemType::Door;
    case LevelTileType::ITEM_FLAMINGO:
        return ItemType::ToyFlamingo;

    case LevelTileType::NONE:
    case LevelTileType::TILE:
    case LevelTileType::AIRFLOW_UP:
    case LevelTileType::AIRFLOW_DOWN:
    case LevelTileType::AIRFLOW_RIGHT:
    case LevelTileType::AIRFLOW_LEFT:
    case LevelTileType::ENEMY_CAN_LEFT:
    case LevelTileType::ENEMY_CAN_RIGHT:
    case LevelTileType::ENEMY_GHOST_LEFT:
    case LevelTileType::ENEMY_GHOST_RIGHT:
    case LevelTileType::ENEMY_PURPLE_LEFT:
    case LevelTileType::ENEMY_PURPLE_RIGHT:
    case LevelTileType::ENEMY_PIG_LEFT:
    case LevelTileType::ENEMY_PIG_RIGHT:
    case LevelTileType::ENEMY_MUSHROOM_LEFT:
    case LevelTileType::ENEMY_MUSHROOM_RIGHT:
    case LevelTileType::ENEMY_SNOWMAN_LEFT:
    case LevelTileType::ENEMY_SNOWMAN_RIGHT:
    case LevelTileType::ENEMY_POTATO_LEFT:
    case LevelTileType::ENEMY_POTATO_RIGHT:
    case LevelTileType::ENEMY_WITCH_LEFT:
    case LevelTileType::ENEMY_WITCH_RIGHT:
        DBG_CHECK(false, "Tried to create item from non item tile");
        return GetItemTypeFromTile(LevelTileType::ITEM_MEAL);
    }
}
