#pragma once

// These values should match with the elements in the level layout json
enum class LevelTileType {
    NONE = 0,
    TILE = 1,
    AIRFLOW_UP = 2,
    AIRFLOW_DOWN = 3,
    AIRFLOW_RIGHT = 4,
    AIRFLOW_LEFT = 5,
    ENEMY_CAN = 6
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
