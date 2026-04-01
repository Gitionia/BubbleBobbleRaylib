#pragma once

// Do not change order. It corresponds to the order of the tiles in tilesets of Tiled
enum class LevelTileType {
    NONE,
    TILE,

    AIRFLOW_UP,
    AIRFLOW_DOWN,
    AIRFLOW_RIGHT,
    AIRFLOW_LEFT,

    ENEMY_CAN_LEFT,
    ENEMY_CAN_RIGHT,
    ENEMY_GHOST_LEFT,
    ENEMY_GHOST_RIGHT,
    ENEMY_PURPLE_LEFT,
    ENEMY_PURPLE_RIGHT,
    ENEMY_PIG_LEFT,
    ENEMY_PIG_RIGHT,
    ENEMY_MUSHROOM_LEFT,
    ENEMY_MUSHROOM_RIGHT,
    ENEMY_SNOWMAN_LEFT,
    ENEMY_SNOWMAN_RIGHT,
    ENEMY_POTATO_LEFT,
    ENEMY_POTATO_RIGHT,
    ENEMY_WITCH_LEFT,
    ENEMY_WITCH_RIGHT,
};

class LevelTilemap {
  public:
    explicit LevelTilemap();
    ~LevelTilemap();

    LevelTileType Get(int x, int y, bool checked = false) const;
    bool IsEmpty(int x, int y) const;

  private:
    void set(int x, int y, LevelTileType type);
    void set(int index, LevelTileType type);
    static int idx(int x, int y);

  public:
    static int OutOfRange(int x, int y);

  private:
    std::vector<LevelTileType> data;

  public:
    static constexpr int WIDTH = 28;
    static constexpr int HEIGHT = 26;
    static constexpr int SIZE = WIDTH * HEIGHT;

    friend class LevelLayout;
};

class LevelLayout {
  public:
    LevelLayout() = default;

    void SetColors(Color right, Color bottem);
    Color GetShadeColorRight() const;
    Color GetShadeColorBottem() const;

    const LevelTilemap &GetTiles() const;
    const LevelTilemap &GetAirflow() const;
    const LevelTilemap &GetEnemies() const;

    bool ContainsBoss() const;
    
    static LevelLayout LoadLevel(const std::string &filepath);

  private:
    LevelTilemap tiles;
    LevelTilemap airflow;
    LevelTilemap enemies;
    Color tileShadeColorRight;
    Color tileShadeColorBottem;

    bool containsBoss = false;
};
