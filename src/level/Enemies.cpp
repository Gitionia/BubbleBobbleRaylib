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
        case EnemyType::GHOST:
            return "Ghost-Walk";
        case EnemyType::PURPLE_GHOST:
            return "Purple-Fly";
        case EnemyType::PIG:
            return "Pig-Fly";
        case EnemyType::MUSHROOM:
            return "Mushroom-Jump";
        case EnemyType::POTATO:
            return "Potato-Walk";
        case EnemyType::WITCH:
            return "Witch-Walk";
        case EnemyType::SNOWMAN:
            return "Snowman-Walk";
        }
        break;

    case EnemyAnimationType::SHOOTING:
        switch (enemyType) {

        case EnemyType::GHOST:
            return "Ghost-Shoot";
        case EnemyType::POTATO:
            return "Potato-Walk"; // Potato has no special animation for shooting
        case EnemyType::WITCH:
            return "Witch-Shoot";
        case EnemyType::SNOWMAN:
            PRINT_ERROR("Unimplemented {}:{}", __FILE__, __LINE__);
            return "Ghost-Shoot";

        case EnemyType::CAN:
        case EnemyType::PURPLE_GHOST:
        case EnemyType::PIG:
        case EnemyType::MUSHROOM:
            PRINT_ERROR("Enemy {} has no shooting animation {}:{}", (int)enemyType, __FILE__, __LINE__);
            return "Ghost-Shoot";
        }
        break;

    case EnemyAnimationType::BUBBLED:
        switch (enemyType) {

        case EnemyType::CAN:
            return "Can-Bubbled";
        case EnemyType::GHOST:
            return "Ghost-Bubbled";
        case EnemyType::PURPLE_GHOST:
            return "Purple-Bubbled";
        case EnemyType::PIG:
            return "Pig-Bubbled";
        case EnemyType::MUSHROOM:
            return "Mushroom-Bubbled";
        case EnemyType::POTATO:
            return "Potato-Bubbled";
        case EnemyType::WITCH:
            return "Witch-Bubbled";
        case EnemyType::SNOWMAN:
            return "Snowman-Bubbled";
        }
        break;

    case EnemyAnimationType::ITEM:
        switch (enemyType) {

        case EnemyType::CAN:
            return "Can-Item";
        case EnemyType::GHOST:
            return "Ghost-Item";
        case EnemyType::PURPLE_GHOST:
            return "Purple-Item";
        case EnemyType::PIG:
            return "Pig-Item";
        case EnemyType::MUSHROOM:
            return "Mushroom-Item";
        case EnemyType::POTATO:
            return "Potato-Item";
        case EnemyType::WITCH:
            return "Witch-Item";
        case EnemyType::SNOWMAN:
            return "Snowman-Item";
        }
        break;
    }
}
const char *GetEnemyProjectileAnimationName(EnemyType shooterEnemy) {
    switch (shooterEnemy) {

    case EnemyType::GHOST:
        return "Projectile-Ghost";
    case EnemyType::POTATO:
        return "Projectile-Potato";
    case EnemyType::WITCH:
        return "Projectile-Witch";
    case EnemyType::SNOWMAN:
        PRINT_ERROR("Unimplemented {}:{}", __FILE__, __LINE__);
        return GetEnemyProjectileAnimationName(EnemyType::GHOST);

    default:
        PRINT_ERROR("Projectile of enemy of type {} has no animation");
        return GetEnemyProjectileAnimationName(EnemyType::GHOST);
    }
}
