#pragma once


#include "../behavior/Level.h"

class EntityFactory {
  public:
    static entt::entity CreateTile(int x, int y, Color shadowRight, Color shadowBottem, bool addShadowRight = true, bool addShadowBottem = true);
    static entt::entity CreateDragon();
    static entt::entity CreateEnemy(int x, int y);
    static entt::entity CreateBubbleCenteredAt(const Vector2Int &centre, int direction);
    static void CreateLevel(const LevelLayout &level);

  private:
    static EntityFactory &get();
    void setRegistry(entt::registry &registry);
    entt::registry *registry;

    friend class Application;
};
