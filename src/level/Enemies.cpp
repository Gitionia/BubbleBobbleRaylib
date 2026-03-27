#include "Enemies.h"

EnemyType GetEnemyTypeFromTile(LevelTileType tile) {
    switch (tile) {

    case LevelTileType::ENEMY_CAN:
        return EnemyType::CAN;
    case LevelTileType::ENEMY_GHOST:
        return EnemyType::GHOST;
    case LevelTileType::ENEMY_PURPLE:
        return EnemyType::PURPLE_GHOST;
    case LevelTileType::ENEMY_PIG:
        return EnemyType::PIG;
    case LevelTileType::ENEMY_MUSHROOM:
        return EnemyType::MUSHROOM;
    case LevelTileType::ENEMY_SNOWMAN:
        return EnemyType::SNOWMAN;
    case LevelTileType::ENEMY_POTATO:
        return EnemyType::POTATO;
    case LevelTileType::ENEMY_WITCH:
        return EnemyType::WITCH;
    default:
        PRINT_ERROR("Tried to convert non-enemy tile into enemy tile. Maybe a level tilemap is incorrect?");
        return EnemyType::CAN;
    }
}