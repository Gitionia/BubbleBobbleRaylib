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

const char *GetEnemyAnimationName(EnemyType enemyType, EnemyAnimationType animationType) {
    switch (animationType) {

    case EnemyAnimationType::NORMAL:
        switch (enemyType) {

        case EnemyType::CAN:
            return "Can-Walk";
        case EnemyType::PURPLE_GHOST:
            return "Purple-Fly";
        case EnemyType::GHOST:
        case EnemyType::PIG:
        case EnemyType::MUSHROOM:
        case EnemyType::SNOWMAN:
        case EnemyType::POTATO:
        case EnemyType::WITCH:
            PRINT_ERROR("Unimplemented {}:{}", __FILE__, __LINE__);
            return "Can-Walk";
        }
    case EnemyAnimationType::BUBBLED:
        switch (enemyType) {

        case EnemyType::CAN:
            return "Can-Bubbled";
        case EnemyType::PURPLE_GHOST:
            return "Purple-Bubbled";
        case EnemyType::GHOST:
        case EnemyType::PIG:
        case EnemyType::MUSHROOM:
        case EnemyType::SNOWMAN:
        case EnemyType::POTATO:
        case EnemyType::WITCH:
            PRINT_ERROR("Unimplemented {}:{}", __FILE__, __LINE__);
            return "Can-Bubbled";
        }
    case EnemyAnimationType::ITEM:
        switch (enemyType) {

        case EnemyType::CAN:
            return "Can-Item";
        case EnemyType::PURPLE_GHOST:
            return "Purple-Item";
        case EnemyType::GHOST:
        case EnemyType::PIG:
        case EnemyType::MUSHROOM:
        case EnemyType::SNOWMAN:
        case EnemyType::POTATO:
        case EnemyType::WITCH:
            PRINT_ERROR("Unimplemented {}:{}", __FILE__, __LINE__);
            return "Can-Item";
        }
        break;
    }
}