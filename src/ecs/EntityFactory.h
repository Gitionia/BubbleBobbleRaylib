#pragma once


#include "../level/Level.h"
#include "Components.h"
#include "../level/Enemies.h"

class EntityFactory {
  public:
    static entt::entity CreateTile(int x, int y, Color shadowRight, Color shadowBottem, bool addShadowRight = true, bool addShadowBottem = true);
    static entt::entity CreateDragon();
    static entt::entity CreateEnemy(int x, int y, EnemyType type);
    static entt::entity CreateTumblingEnemy(int x, int y, int dir, ItemType itemType);
    static entt::entity CreateItem(const Vector2Int& pos, ItemType itemType);
    static entt::entity CreateItemPointsText(Vector2Int pos, ItemType itemType);
    static entt::entity CreateBubbleCenteredAt(const Vector2Int &centre, int direction);
    static entt::entity CreateModifiableUIText(const Vector2Int &position, const std::string& text, Color color, int fontSize = 32, int spacing = 0);
    static entt::entity CreateConstantUIText(const Vector2Int &position, const char* text, Color color, int fontSize = 32, int spacing = 0);
    static entt::entity CreateSimpleSprite(const Vector2Int &position, int dir, SpriteHandle sprite, Color color, Vector2 scale);
    static void CreateLevel(const LevelLayout &level);

  private:
    static EntityFactory &get();
    void setRegistry(entt::registry &registry);
    entt::registry *registry;

    friend class Application;
};
