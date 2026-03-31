#include "EntityFactory.h"

#include "Components.h"

entt::entity EntityFactory::CreateTile(int x, int y, Color colorShadowRight, Color colorShadowBottem, int level, bool addShadowRight, bool addShadowBottem) {
    entt::registry *registry = get().registry;
    auto tile = registry->create();
    {
        registry->emplace<Position>(tile, x * UNITS_PER_BLOCK, y * UNITS_PER_BLOCK);
        RenderData data = {GetSpriteHandle(std::format("Block-Level{}", level % 100)), {1, 1}};
        data.scale = {2, 2};
        registry->emplace<RenderData>(tile, data);
        registry->emplace<LevelTileTag>(tile);

        registry->emplace<GameplayEntityTag>(tile);
    }

    if (addShadowRight) {
        auto shadowRight = registry->create();
        registry->emplace<Position>(shadowRight, (x + 1) * UNITS_PER_BLOCK, y * UNITS_PER_BLOCK);
        RenderData data = {GetSpriteHandle("TileShadowRight"), {2, 2}};
        data.SetColor(colorShadowRight);
        registry->emplace<RenderData>(shadowRight, data);
        registry->emplace<LevelTileShadowTag>(shadowRight);

        registry->emplace<GameplayEntityTag>(shadowRight);
    }
    if (addShadowBottem) {
        auto shadowBottem = registry->create();
        registry->emplace<Position>(shadowBottem, x * UNITS_PER_BLOCK, (y + 1) * UNITS_PER_BLOCK);
        RenderData data = {GetSpriteHandle("TileShadowBottem"), {2, 2}};
        data.SetColor(colorShadowBottem);
        registry->emplace<RenderData>(shadowBottem, data);
        registry->emplace<LevelTileShadowTag>(shadowBottem);

        registry->emplace<GameplayEntityTag>(shadowBottem);
    }

    return tile;
}

entt::entity EntityFactory::CreateDragon(bool withInvincibility) {
    entt::registry *registry = get().registry;
    auto dragon = registry->create();

    registry->emplace<Position>(dragon, DragonComponent::STARTING_POSITION);
    registry->emplace<RenderData>(dragon, RenderData(GetSpriteHandle("Dragon-Idle-1"), {2, 2}));
    registry->emplace<WalkingActorComponent>(dragon, DragonComponent::FALL_SPEED, DragonComponent::JUMP_SPEED);
    auto &dragonComp = registry->emplace<DragonComponent>(dragon);
    if (withInvincibility) {
        dragonComp.invincibilityFramesLeft = DragonComponent::INVINCIBILITY_FRAME_COUNT;
    }
    registry->emplace<DragonTag>(dragon);

    registry->emplace<GameplayEntityTag>(dragon);

    return dragon;
}

entt::entity EntityFactory::CreateBubbleCenteredAt(const Vector2Int &centre, int direction) {
    entt::registry *registry = get().registry;
    auto bubble = registry->create();

    registry->emplace<Position>(bubble, centre.X - BP_SIZE(0, 14), centre.Y - BP_SIZE(1, 0));
    registry->emplace<RenderData>(bubble, RenderData(GetSpriteHandle("Bubble-Green-Idle-1"), {2, 2}));
    registry->emplace<BubbleShootComponent>(bubble, direction);
    registry->emplace<BubbleTag>(bubble);

    registry->emplace<GameplayEntityTag>(bubble);

    return bubble;
}

entt::entity EntityFactory::CreateEnemy(int x, int y, EnemyType type, Direction direction) {
    entt::registry *registry = get().registry;
    auto enemy = registry->create();

    registry->emplace<Position>(enemy, x, y, GetIntFromDirection(direction));
    // TODO: Don't need to set the renderdata, because it gets overwritten anyways. Replace in the future
    registry->emplace<RenderData>(enemy, RenderData(GetSpriteHandle(GetEnemyAnimationName(type, EnemyAnimationType::NORMAL) + std::string("-1")), {2, 2}));
    switch (type) {

    case EnemyType::CAN:
    case EnemyType::GHOST:
    case EnemyType::MUSHROOM:
    case EnemyType::POTATO:
    case EnemyType::SNOWMAN:
    case EnemyType::WITCH: {
    case EnemyType::BOSS:
        registry->emplace<WalkingEnemyComponent>(enemy, GetIntFromDirection(direction), &GetAnimation(GetEnemyAnimationName(type, EnemyAnimationType::NORMAL)));
        registry->emplace<WalkingActorComponent>(enemy, UNITS_PER_BLOCK / 16, 3 * UNITS_PER_BLOCK / 16);
        break;
    }
    case EnemyType::PURPLE_GHOST:
    case EnemyType::PIG:
        registry->emplace<FlyingEnemyComponent>(enemy, direction == Direction::Left ? FlyingEnemyComponent::DOWN_LEFT : FlyingEnemyComponent::DOWN_RIGHT);
        break;
    }

    registry->emplace<EnemyInfoComponent>(enemy, type);
    registry->emplace<EnemyTag>(enemy);

    registry->emplace<GameplayEntityTag>(enemy);

    return enemy;
}

entt::entity EntityFactory::CreateEnemyProjectile(int x, int y, int dir, EnemyType shooterEnemy) {
    entt::registry *registry = get().registry;
    auto projectile = registry->create();

    registry->emplace<Position>(projectile, x, y, dir);
    // TODO: Don't need to set the renderdata, because it gets overwritten anyways. Replace in the future
    registry->emplace<RenderData>(projectile, RenderData(GetSpriteHandle("Projectile-Ghost-1"), {2, 2}));

    registry->emplace<EnemyProjectileComponent>(projectile, shooterEnemy, &GetAnimation(GetEnemyProjectileAnimationName(shooterEnemy)));
    registry->emplace<EnemyProjectileTag>(projectile);

    registry->emplace<GameplayEntityTag>(projectile);

    return projectile;
}

entt::entity EntityFactory::CreateTumblingEnemy(int x, int y, int dir, EnemyType enemyType, ItemType itemType) {
    entt::registry *registry = get().registry;
    auto enemy = registry->create();

    int s = 20;
    static AnimatedValueDefinition<int> xVelLow{{BP_SIZE(0, 0), 10}, {BP_SIZE(0, 1), 10}, {BP_SIZE(0, 2), s}, {BP_SIZE(0, 3), 15}, {BP_SIZE(0, 2), s}, {BP_SIZE(0, 1), 10}, {BP_SIZE(0, 0), 10}};
    static AnimatedValueDefinition<int> yVelLow{{BP_SIZE(0, 3), s}, {BP_SIZE(0, 2), s}, {BP_SIZE(0, 1), 10}, {BP_SIZE(0, 0), 5}, {-BP_SIZE(0, 1), 10}, {-BP_SIZE(0, 2), s}, {-BP_SIZE(0, 3), s}};
    s = 30;
    static AnimatedValueDefinition<int> xVelHigh{{BP_SIZE(0, 0), 15}, {BP_SIZE(0, 1), 15}, {BP_SIZE(0, 2), s}, {BP_SIZE(0, 3), 30}, {BP_SIZE(0, 2), s}, {BP_SIZE(0, 1), 15}, {BP_SIZE(0, 0), 15}};
    static AnimatedValueDefinition<int> yVelHigh{{BP_SIZE(0, 3), s}, {BP_SIZE(0, 2), s}, {BP_SIZE(0, 1), 15}, {BP_SIZE(0, 0), 15}, {-BP_SIZE(0, 1), 10}, {-BP_SIZE(0, 2), s}, {-BP_SIZE(0, 3), s}};

    registry->emplace<Position>(enemy, x, y, dir);
    // TODO Replace sprite with empty (transparent or 0 area sprite) handle
    registry->emplace<RenderData>(enemy, RenderData(GetSpriteHandle("Can-Item-1"), {2, 2}));

    bool rand = Random::Get().Bool();
    auto &xVel = rand ? xVelHigh : xVelLow;
    auto &yVel = rand ? yVelHigh : yVelLow;
    registry->emplace<EnemyTumbleComponent>(enemy, &GetAnimation(GetEnemyAnimationName(enemyType, EnemyAnimationType::ITEM)), xVel, yVel, itemType);
    registry->emplace<EnemyTumbleTag>(enemy);

    registry->emplace<GameplayEntityTag>(enemy);

    return enemy;
}

entt::entity EntityFactory::CreateItem(const Vector2Int &pos, ItemType itemType) {
    entt::registry *registry = get().registry;

    auto item = registry->create();
    registry->emplace<Position>(item, pos.X, pos.Y);
    registry->emplace<RenderData>(item, RenderData(GetSpriteHandle(GetItemSpriteName(itemType)), {2, 2}));
    registry->emplace<ItemComponent>(item, itemType);
    registry->emplace<ItemTag>(item);

    registry->emplace<GameplayEntityTag>(item);

    return item;
}

entt::entity EntityFactory::CreateItemPointsText(Vector2Int pos, ItemType itemType) {
    entt::registry *registry = get().registry;

    pos.Y += BP_SIZE(0, 8);

    auto entity = registry->create();
    registry->emplace<Position>(entity, pos.X, pos.Y);
    registry->emplace<RenderData>(entity,
                                  RenderData(GetSpriteHandle(GetPointTextSpriteNameFromItemType(itemType)), {2, 2}));
    registry->emplace<PositionAnimationComponent>(entity, pos, pos.Add(0, BP_SIZE(-5, 0)),
                                                  120, PositionAnimationComponent::DELETE_ENTITY);
    registry->emplace<InGameTextTag>(entity);

    registry->emplace<GameplayEntityTag>(entity);

    return entity;
}

entt::entity EntityFactory::CreateModifiableUIText(const Vector2Int &pos, const std::string &text, Color color, int fontSize, int spacing) {
    entt::registry *registry = get().registry;

    auto entity = registry->create();
    registry->emplace<Position>(entity, pos.X, pos.Y);
    registry->emplace<ModifiableUITextComponent>(entity, text, color, fontSize, spacing);

    registry->emplace<UITextEntityTag>(entity);

    return entity;
}

entt::entity EntityFactory::CreateConstantUIText(const Vector2Int &pos, const char *text, Color color, int fontSize, int spacing) {
    entt::registry *registry = get().registry;

    auto entity = registry->create();
    registry->emplace<Position>(entity, pos.X, pos.Y);
    registry->emplace<ConstUITextComponent>(entity, text, color, fontSize, spacing);

    registry->emplace<UITextEntityTag>(entity);

    return entity;
}

entt::entity EntityFactory::CreateSimpleSprite(const Vector2Int &pos, int dir, SpriteHandle sprite, Color color, Vector2 scale) {
    entt::registry *registry = get().registry;

    auto entity = registry->create();
    registry->emplace<Position>(entity, pos.X, pos.Y, dir);
    RenderData data(sprite, scale);
    registry->emplace<RenderData>(entity, data.SetColor(color));
    registry->emplace<SimpleSpriteTag>(entity);

    registry->emplace<SpriteEntityTag>(entity);

    return entity;
}

void EntityFactory::CreateLevel(const LevelLayout &level, int levelNumber) {
    for (int x = 2; x < LevelTilemap::WIDTH + 2; ++x) {
        for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
            bool addShadowRight = LevelTilemap::OutOfRange((x - 2) + 1, y) ? true : level.GetTiles().IsEmpty((x - 2) + 1, y);
            bool addShadowBottem = LevelTilemap::OutOfRange((x - 2), y + 1) ? true : level.GetTiles().IsEmpty((x - 2), y + 1);
            if (!level.GetTiles().IsEmpty(x - 2, y)) {
                CreateTile(x, y, level.GetShadeColorRight(), level.GetShadeColorBottem(), levelNumber, addShadowRight, addShadowBottem);
            }
        }
    }

    int rows[] = {0, 1, 30, 31};
    for (int x : rows) {
        for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
            bool addShadowRight = x == 1;
            bool addShadowBottem = false;
            CreateTile(x, y, level.GetShadeColorRight(), level.GetShadeColorBottem(), levelNumber, addShadowRight, addShadowBottem);
        }
    }

    for (int x = 0; x < LevelTilemap::WIDTH; ++x) {
        for (int y = 0; y < LevelTilemap::HEIGHT; ++y) {
            if (!level.GetEnemies().IsEmpty(x, y)) {
                LevelTileType tile = level.GetEnemies().Get(x, y);
                EnemyType type = GetEnemyTypeFromTile(tile);
                Direction direction = GetEnemyDirectionFromTile(tile);
                CreateEnemy(BP_SIZE(x + 2, 0), BP_SIZE(y, 0), type, direction);
            }
        }
    }
}

EntityFactory &EntityFactory::get() {
    static EntityFactory factory;
    return factory;
}

void EntityFactory::setRegistry(entt::registry &registry) {
    this->registry = &registry;
}
