#include "Enemies.h"

EnemyType GetEnemyTypeFromTile(LevelTileType tile) {
    switch (tile) {

    case LevelTileType::ENEMY_CAN_LEFT:
    case LevelTileType::ENEMY_CAN_RIGHT:
        return EnemyType::CAN;
    case LevelTileType::ENEMY_GHOST_LEFT:
    case LevelTileType::ENEMY_GHOST_RIGHT:
        return EnemyType::GHOST;
    case LevelTileType::ENEMY_PURPLE_LEFT:
    case LevelTileType::ENEMY_PURPLE_RIGHT:
        return EnemyType::PURPLE_GHOST;
    case LevelTileType::ENEMY_PIG_LEFT:
    case LevelTileType::ENEMY_PIG_RIGHT:
        return EnemyType::PIG;
    case LevelTileType::ENEMY_MUSHROOM_LEFT:
    case LevelTileType::ENEMY_MUSHROOM_RIGHT:
        return EnemyType::MUSHROOM;
    case LevelTileType::ENEMY_SNOWMAN_LEFT:
    case LevelTileType::ENEMY_SNOWMAN_RIGHT:
        return EnemyType::SNOWMAN;
    case LevelTileType::ENEMY_POTATO_LEFT:
    case LevelTileType::ENEMY_POTATO_RIGHT:
        return EnemyType::POTATO;
    case LevelTileType::ENEMY_WITCH_LEFT:
    case LevelTileType::ENEMY_WITCH_RIGHT:
        return EnemyType::WITCH;
    default:
        PRINT_ERROR("Tried to convert non-enemy tile into enemy tile. Maybe a level tilemap is incorrect?");
        return EnemyType::CAN;
    }
}

Direction GetEnemyDirectionFromTile(LevelTileType tile) {
    switch (tile) {

    case LevelTileType::ENEMY_CAN_LEFT:
    case LevelTileType::ENEMY_PURPLE_LEFT:
    case LevelTileType::ENEMY_GHOST_LEFT:
    case LevelTileType::ENEMY_PIG_LEFT:
    case LevelTileType::ENEMY_MUSHROOM_LEFT:
    case LevelTileType::ENEMY_SNOWMAN_LEFT:
    case LevelTileType::ENEMY_POTATO_LEFT:
    case LevelTileType::ENEMY_WITCH_LEFT:
    return Direction::Left;
    case LevelTileType::ENEMY_CAN_RIGHT:
    case LevelTileType::ENEMY_GHOST_RIGHT:
    case LevelTileType::ENEMY_PURPLE_RIGHT:
    case LevelTileType::ENEMY_PIG_RIGHT:
    case LevelTileType::ENEMY_MUSHROOM_RIGHT:
    case LevelTileType::ENEMY_SNOWMAN_RIGHT:
    case LevelTileType::ENEMY_POTATO_RIGHT:
    case LevelTileType::ENEMY_WITCH_RIGHT:
    return Direction::Right;
    default:
        PRINT_ERROR("Tried to get direction of non-enemy tile. Maybe a level tilemap is incorrect?");
        return Direction::Left;
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
        case EnemyType::PIG:
            return "Pig-Fly";
        case EnemyType::MUSHROOM:
            return "Mushroom-Jump";
        case EnemyType::GHOST:
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
        case EnemyType::PIG:
            return "Pig-Bubbled";
        case EnemyType::MUSHROOM:
            return "Mushroom-Bubbled";
        case EnemyType::GHOST:
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
        case EnemyType::PIG:
            return "Pig-Item";
        case EnemyType::MUSHROOM:
            return "Mushroom-Item";
        case EnemyType::GHOST:
        case EnemyType::SNOWMAN:
        case EnemyType::POTATO:
        case EnemyType::WITCH:
            PRINT_ERROR("Unimplemented {}:{}", __FILE__, __LINE__);
            return "Can-Item";
        }
        break;
    }
}
