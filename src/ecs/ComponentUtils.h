#pragma once

#define COLLIDER_LAYER(name)                                                     \
    struct name {                                                                \
        int width, height;                                                       \
        int offsetX, offsetY;                                                    \
        /*Flips this collider on the x-Axis of an entity with width 'boxwidth'*/ \
        void flipX(int boxWidth) {                                               \
            offsetX = boxWidth - (offsetX + width);                              \
        }                                                                        \
        Vector2Int getCenter(int x, int y) const {                               \
            return {x + width / 2, y + height / 2};                              \
        }                                                                        \
        int left(Vector2Int pos) const {                                         \
            return pos.X + offsetX;                                              \
        }                                                                        \
        int right(Vector2Int pos) const {                                        \
            return pos.X + width + offsetX;                                      \
        }                                                                        \
        int top(Vector2Int pos) const {                                          \
            return pos.Y + offsetY;                                              \
        }                                                                        \
        int bottem(Vector2Int pos) const {                                       \
            return pos.Y + height + offsetY;                                     \
        }                                                                        \
    };

COLLIDER_LAYER(Collider)

#define MULTI_COLLIDER_LAYER(name, count)     \
    struct name {                             \
        Collider colliders[count];            \
        void flipX(int boxWidth) {            \
            for (Collider &col : colliders) { \
                col.flipX(boxWidth);          \
            }                                 \
        }                                     \
    };


void MakeBubbleAndGroupPopFromLifetime(entt::registry &registry, entt::entity entity);
void MakeBubbleAndGroupPopFromDragonSpikes(entt::registry &registry, entt::entity entity);

