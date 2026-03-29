#include "Level.h"

LevelTilemap::LevelTilemap()
    : data(SIZE) {
}

LevelTilemap::~LevelTilemap() {
}

LevelTileType LevelTilemap::Get(int x, int y, bool checked) const {
    if (checked && OutOfRange(x, y))
        return LevelTileType::NONE;
    return data.at(idx(x, y));
}

bool LevelTilemap::IsEmpty(int x, int y) const {
    return Get(x, y) == LevelTileType::NONE;
}

void LevelTilemap::set(int x, int y, LevelTileType type) {
    data.at(idx(x, y)) = type;
}

void LevelTilemap::set(int index, LevelTileType type) {
    data.at(index) = type;
}

int LevelTilemap::idx(int x, int y) {
    return x + y * WIDTH;
}

int LevelTilemap::OutOfRange(int x, int y) {
    return x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT;
}

void LevelLayout::SetColors(Color right, Color bottem) {
    tileShadeColorRight = right;
    tileShadeColorBottem = bottem;
}

Color LevelLayout::GetShadeColorRight() const {
    return tileShadeColorRight;
}

Color LevelLayout::GetShadeColorBottem() const {
    return tileShadeColorBottem;
}

const LevelTilemap &LevelLayout::GetTiles() const {
    return tiles;
}

const LevelTilemap &LevelLayout::GetAirflow() const {
    return airflow;
}

const LevelTilemap &LevelLayout::GetEnemies() const {
    return enemies;
}

unsigned int parseColorChannel(const std::string &str) {
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;
    return x;
}

Color parseColors(const std::string &s) {
    unsigned char a = parseColorChannel(s.substr(1, 2));
    unsigned char r = parseColorChannel(s.substr(3, 2));
    unsigned char g = parseColorChannel(s.substr(5, 2));
    unsigned char b = parseColorChannel(s.substr(7, 2));

    return {r, g, b, a};
}

static LevelTileType flipAirflowX(LevelTileType tile) {
    switch (tile) {

    case LevelTileType::NONE:
        return LevelTileType::NONE;

    case LevelTileType::AIRFLOW_UP:
    case LevelTileType::AIRFLOW_DOWN:
        return tile;

    case LevelTileType::AIRFLOW_RIGHT:
        return LevelTileType::AIRFLOW_LEFT;
    case LevelTileType::AIRFLOW_LEFT:
        return LevelTileType::AIRFLOW_RIGHT;

    default:
        PRINT_WARN("Tried to flip a non airflow tile '{}' which is not allowed! {}:{}", (int)tile, __FILE__, __LINE__);
        return LevelTileType::AIRFLOW_UP;
    }
}

LevelLayout LevelLayout::LoadLevel(const std::string &filepath) {
    LevelLayout level{};

    std::ifstream f(filepath);
    if (f.fail()) {
        PRINT_CRITICAL("Level data at {} is missing!", filepath.c_str());
        return level;
    }

    nlohmann::json data = nlohmann::json::parse(f);

    // Parse tileset gids
    auto tilesets = data.find("tilesets").value();

    enum TileSetType {
        TS_BLOCK,
        TS_AIRFLOW,
        TS_ENEMY,
        TILESET_TYPE_COUNT
    };
    int gids[TILESET_TYPE_COUNT];
    DBG_CHECK(tilesets.size() == TILESET_TYPE_COUNT, "Unexpected count of used tilesets at " + std::string(filepath.c_str()));
    for (auto &tileset : tilesets) {

        auto x = tileset.find("gid");
        std::string source = tileset.find("source").value();
        int gid = tileset.find("firstgid").value();

        if (source.find("LevelBlocks.tsx") != std::string::npos) {
            gids[TS_BLOCK] = gid;
        } else if (source.find("AirflowTileset.tsx") != std::string::npos) {
            gids[TS_AIRFLOW] = gid;
        } else if (source.find("EnemyTiles.tsx") != std::string::npos) {
            gids[TS_ENEMY] = gid;
        } else {
            PRINT_WARN("{} contains invalid tileset {}", filepath.c_str(), source);
        }
    }

    // Parse layers
    auto layers = data.find("layers").value();

    for (auto &layer : layers) {
        if (layer.find("name").value() == "Tiles") {
            auto levelData = layer.find("data").value();

            auto properties = layer.find("properties").value();
            std::string colorRight;
            std::string colorBottem;
            bool shouldFlipAlongXAxis = false;

            if (layer.contains("properties")) {
                for (auto &property : properties) {

                    if (property.find("name").value() == "ShadeBottom") {
                        colorBottem = property.find("value").value();
                    } else if (property.find("name").value() == "ShadeRight") {
                        colorRight = property.find("value").value();

                    } else if (property.find("name").value() == "FlipAlongXAxis") {
                        shouldFlipAlongXAxis = property.find("value").value();

                    } else {
                        PRINT_ERROR("Unexpected property {} at {} in Tiles layer", std::string(property.find("name").value()), filepath.c_str());
                    }
                }
            }

            if (colorRight.empty() || colorBottem.empty()) {
                PRINT_ERROR("Shade Colors of Level at {} is missing!", filepath.c_str());
                level.SetColors(BLACK, BLACK);
            } else {
                level.SetColors(parseColors(colorRight), parseColors(colorBottem));
            }

            if (levelData.size() != 26 * 28)
                PRINT_ERROR("Level at {} contains leveldata with invalid length of {} instead of {} on layer 'Tiles'",
                            filepath.c_str(), levelData.size(), 26 * 28);

            for (int i = 0; i < levelData.size(); i++) {
                level.tiles.set(i, levelData.at(i) > 0 ? LevelTileType::TILE : LevelTileType::NONE);
            }

            if (shouldFlipAlongXAxis) {
                for (int x = 0; x < LevelTilemap::WIDTH / 2; x++) {
                    for (int y = 0; y < LevelTilemap::HEIGHT; y++) {
                        bool isEmpty = level.tiles.Get(x, y) == LevelTileType::NONE;
                        int index = LevelTilemap::idx(LevelTilemap::WIDTH - 1 - x, y);
                        level.tiles.set(index, isEmpty ? LevelTileType::NONE : LevelTileType::TILE);
                    }
                }
            }

        } else if (layer.find("name").value() == "Airflow") {
            auto levelData = layer.find("data").value();

            bool shouldFlipAlongXAxis = false;
            if (layer.contains("properties")) {
                auto properties = layer.find("properties").value();
                for (auto &property : properties) {
                    if (property.find("name").value() == "FlipAlongXAxis") {
                        shouldFlipAlongXAxis = property.find("value").value();
                    } else {
                        PRINT_ERROR("Unexpected property {} at {} in Airflow layer", std::string(property.find("name").value()), filepath.c_str());
                    }
                }
            }

            if (levelData.size() != 26 * 28)
                PRINT_ERROR("Level at {} contains leveldata with invalid length of {} instead of {} on layer 'Airflow'",
                            filepath.c_str(), levelData.size(), 26 * 28);

            for (int i = 0; i < levelData.size(); i++) {
                int value = levelData.at(i);
                int type;
                if (value == 0)
                    type = 0; // None
                else
                    type = value - gids[TS_AIRFLOW] + (int)LevelTileType::AIRFLOW_UP;

                level.airflow.set(i, (LevelTileType)type);
            }

            if (shouldFlipAlongXAxis) {
                for (int x = 0; x < LevelTilemap::WIDTH / 2; x++) {
                    for (int y = 0; y < LevelTilemap::HEIGHT; y++) {
                        LevelTileType val = flipAirflowX(level.airflow.Get(x, y));
                        int index = LevelTilemap::idx(LevelTilemap::WIDTH - 1 - x, y);
                        level.airflow.set(index, val);
                    }
                }
            }

        } else if (layer.find("name").value() == "Enemies") {
            auto levelData = layer.find("data").value();

            bool shouldFlipAlongXAxis = false;
            if (layer.contains("properties")) {
                auto properties = layer.find("properties").value();
                for (auto &property : properties) {

                    if (property.find("name").value() == "FlipAlongXAxis") {
                        shouldFlipAlongXAxis = property.find("value").value();

                    } else {
                        PRINT_ERROR("Unexpected property {} at {} in Enemy layer", std::string(property.find("name").value()), filepath.c_str());
                    }
                }
            }

            if (levelData.size() != 26 * 28)
                PRINT_ERROR("Level at {} contains leveldata with invalid length of {} instead of {} on layer 'Enemies'",
                            filepath.c_str(), levelData.size(), 26 * 28);

            for (int i = 0; i < levelData.size(); i++) {
                int value = levelData.at(i);
                int type;
                if (value == 0)
                    type = 0; // None
                else
                    type = value - gids[TS_ENEMY] + (int)LevelTileType::ENEMY_CAN;

                level.enemies.set(i, levelData.at(i));
            }

            if (shouldFlipAlongXAxis) {
                for (int x = 0; x < LevelTilemap::WIDTH / 2; x++) {
                    for (int y = 0; y < LevelTilemap::HEIGHT; y++) {
                        LevelTileType val = level.enemies.Get(x, y);
                        int index = LevelTilemap::idx(LevelTilemap::WIDTH - 1 - x - 1, y); // We do an extra -1, because enemies occupy two spaces, but the tile only one
                        level.enemies.set(index, val);
                    }
                }
            }

        } else {
            PRINT_ERROR("Level at {} contains invalid layer! Expected to be 'Tiles', 'Airflow' or 'Enemies'", filepath.c_str());
        }
    }

    return level;
}
