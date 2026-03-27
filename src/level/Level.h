#pragma once

// Do not change order. It corresponds to the order of the tiles in tilesets of Tiled
enum class LevelTileType {
    NONE,
    TILE,

    AIRFLOW_UP,
    AIRFLOW_DOWN,
    AIRFLOW_RIGHT,
    AIRFLOW_LEFT,

    ENEMY_CAN,
    ENEMY_GHOST,
    ENEMY_PURPLE,
    ENEMY_PIG,
    ENEMY_MUSHROOM,
    ENEMY_SNOWMAN,
    ENEMY_POTATO,
    ENEMY_WITCH,
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

    static LevelLayout LoadLevel(const std::string &filepath);

  private:
    LevelTilemap tiles;
    LevelTilemap airflow;
    LevelTilemap enemies;
    Color tileShadeColorRight;
    Color tileShadeColorBottem;
};
