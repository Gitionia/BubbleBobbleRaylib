#include "RendererSystem.h"

#include "../ecs/Components.h"

#include "../app/Config.h"

void RendererSystem::Init() {
    renderTexture = LoadRenderTexture(TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);
}

RendererSystem::~RendererSystem() {
    UnloadRenderTexture(renderTexture);
}

void RendererSystem::Update() {
    BeginTextureMode(renderTexture);

    ClearBackground(BLACK);

    renderAllWithTag<LevelTileShadowTag>();
    renderAllWithTag<LevelTileTag>();
    renderAllWithTag<EnemyTag>();
    renderAllWithTag<EnemyItemDiceTag>();
    renderAllWithTag<BubbleTag>();
    renderAllWithTag<DragonTag>();

    drawDebugShapes();

#ifdef DRAW_DEBUG
    debugDrawColliders<Collider>(RED);
    debugDrawMultiColliders<DragonSpikeCollider>(BLUE);
    debugDrawColliders<BubbleJumpableTopCollider>(GREEN);
#endif

    DrawFPS(10, 10);

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    Rectangle src = {0, 0, TARGET_WINDOW_WIDTH, -TARGET_WINDOW_HEIGHT};

    float width = GetScreenWidth();
    float height = GetScreenHeight();

    float scaleX = width / TARGET_WINDOW_WIDTH;
    float scaleY = height / TARGET_WINDOW_HEIGHT;

    float scale = std::min(scaleX, scaleY);

    Rectangle dest = {0, 0, TARGET_WINDOW_WIDTH * scale, TARGET_WINDOW_HEIGHT * scale};
    DrawTexturePro(renderTexture.texture, src, dest, {0, 0}, 0, WHITE);
    EndDrawing();
}

void RendererSystem::drawDebugShapes() {
    auto viewRenderer = registry.view<Position, DebugCircle>();
    for (auto entity : viewRenderer) {
        auto [pos, circle] = viewRenderer.get(entity);

        DrawCircle(pos.x * SCALING_FACTOR, pos.y * SCALING_FACTOR, circle.radius, circle.color);
    }

    auto view = registry.view<DebugDrawTag>();
    registry.destroy(view.begin(), view.end());
}

template <typename Tag>
void RendererSystem::renderAllWithTag() {
    auto viewRenderer = registry.view<Position, RenderData, Tag>();
    for (auto entity : viewRenderer) {
        auto [pos, renderData] = viewRenderer.get(entity);

        const Sprite &sprite = GetSprite(renderData.spriteHandle);
        Rectangle sourceRect = sprite.coords;

        if (pos.dir > 0) {
            sourceRect.width *= -1;
        }
        Rectangle dest = {(float)pos.x / UNITS_TO_PIXEL_SCALE + renderData.xoffset, (float)pos.y / UNITS_TO_PIXEL_SCALE + renderData.yoffset,
                          sprite.coords.width * renderData.scale.x, sprite.coords.height * renderData.scale.y};

        dest = OffsetRect(dest, sprite.xOffset, sprite.yOffset);
        dest = ScaleRect(dest, SCALING_FACTOR);

        DrawTexturePro(sprite.spriteSheet, sourceRect,
                       dest,
                       {0, 0}, 0, renderData.color);
    }
}

template <typename ColliderType>
void RendererSystem::debugDrawColliders(Color color) {
    auto viewRenderer = registry.view<Position, ColliderType>();
    for (auto entity : viewRenderer) {
        auto [pos, col] = viewRenderer.get(entity);

        // * 2, because one Block has 32 screen pixels, but has 16 sprite pixels
        DrawRectangle((pos.x + col.offsetX) * 2, (pos.y + col.offsetY) * 2,
                      col.width * 2, col.height * 2, color);
    }
}

template <typename ColliderType>
void RendererSystem::debugDrawMultiColliders(Color color) {
    auto viewRenderer = registry.view<Position, ColliderType>();
    for (auto entity : viewRenderer) {
        auto [pos, multiCollider] = viewRenderer.get(entity);

        for (auto &col : multiCollider.colliders) {
            // * 2, because one Block has 32 screen pixels, but has 16 sprite pixels
            DrawRectangle((pos.x + col.offsetX) * 2, (pos.y + col.offsetY) * 2, col.width * 2, col.height * 2, color);
        }
    }
}
