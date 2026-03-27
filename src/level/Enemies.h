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
};

EnemyType GetEnemyTypeFromTile(LevelTileType tile);

enum class EnemyAnimationType {
    NORMAL,
    BUBBLED,
    ITEM
};
const char* GetEnemyAnimationName(EnemyType enemyType, EnemyAnimationType animationType);