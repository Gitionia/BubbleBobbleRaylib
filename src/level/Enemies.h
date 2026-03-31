#pragma once

#include "Level.h"

enum class EnemyType {
    CAN,
    GHOST,
    PURPLE_GHOST,
    PIG,
    MUSHROOM,
    SNOWMAN,
    POTATO,
    WITCH,
    BOSS
};

EnemyType GetEnemyTypeFromTile(LevelTileType tile);
Direction GetEnemyDirectionFromTile(LevelTileType tile);

enum class EnemyAnimationType {
    NORMAL,
    SHOOTING,
    BUBBLED,
    ITEM
};
const char* GetEnemyAnimationName(EnemyType enemyType, EnemyAnimationType animationType);
const char *GetEnemyProjectileAnimationName(EnemyType shooterEnem);
