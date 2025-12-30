#include "DragonBehaviorSystem.h"

#include <iostream>

#include "../app/Config.h"
#include "../ecs/Components.h"
#include "../graphics/Animations.h"
#include "../utils/Input.h"
#include "Physics.h"

constexpr int dragonJumpingSpeeds[] = {
    0, 2, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0};

constexpr int jumpAnimationLength = sizeof(dragonJumpingSpeeds) / sizeof(int);

void DragonBehaviorSystem::Init() {
}

void DragonBehaviorSystem::Update() {

    static Animator animator(&GetAnimation("Dragon-Idle"));

    auto view = registry.view<Position, WalkingActorComponent, DragonComponent, RenderData, Collider, DragonSpikeCollider>();
    for (auto entity : view) {
        auto [pos, actor, dragon, renderData, collider, dragonSpikes] = view.get(entity);

        if (animator.Update())
            animator.Reset();
        renderData.spriteHandle = animator.GetSpriteHandle();

        bool useGamepad = true;

        int velx = 0;
        int vely = 0;
        int moveSpeed = UNITS_PER_BLOCK / 16;
        int jumpEndSpeed = UNITS_PER_BLOCK / 8;
        int jumpSpeed = 3 * UNITS_PER_BLOCK / 16;
        int fallSpeed = UNITS_PER_BLOCK / 8;
        int JUMP_SLOWDOWN_COUNT = 5 * (int)(UNITS_PER_BLOCK * 0.9f) / jumpSpeed;

        int JUMP_FRAME_COUNT = 5 * (int)(UNITS_PER_BLOCK * 1.3f) / jumpSpeed;

        int BOTTEM_WARP_POS = 27 * UNITS_PER_BLOCK;
        int TOP_WARP_POS = 30 * UNITS_PER_BLOCK;

        bool jump = Input::IsKeyDown(Key::Jump);
        velx = moveSpeed * Input::GetXAxis();

        bool targetFilp;
        if (velx > 0) {
            targetFilp = true;
        } else if (velx < 0) {
            targetFilp = false;
        }

        if (velx != 0) {
            if (targetFilp != renderData.flipX) {
                dragonSpikes.flipX(2 * UNITS_PER_BLOCK);
            }
            renderData.flipX = targetFilp;
        }

        // check if should restore collision detection
        // if (actor.ignoreCollisions) {
        // 	if (!collidesWithWall(registry, pos, collider)) {
        // 		actor.ignoreCollisions = false;
        // 	}
        // }

        if (dragon.bubbleShootDelay == 0 && Input::IsKeyDown(Key::Fire)) {
            EntityFactory::CreateBubbleCenteredAt(pos.toVector().Add(BP_SIZE(1, 0), BP_SIZE(1, 0)), renderData.flipX ? 1 : -1);
            dragon.bubbleShootDelay = dragon.MAX_BUBBLE_SHOOT_DELAY;
        } else if (dragon.bubbleShootDelay > 0) {
            dragon.bubbleShootDelay--;
        }

        // Above and below the level the dragon should ignore collisions.
        // Above includes all y-positions where the dragon would be standing on the
        // top of the level or above that.
        if (pos.y > 24 * UNITS_PER_BLOCK || pos.y <= -2 * UNITS_PER_BLOCK) {
            actor.ignoreCollisions = true;
        } else {
            actor.ignoreCollisions = collidesWithWall(registry, pos, collider);
        }

        // check if grounded
        bool isGrounded = false;
        if (!actor.isJumping) {

            pos.y += fallSpeed;
            if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
                isGrounded = true;
            }
            pos.y -= fallSpeed;
        }

        // start jump
        if (jump) {
            if (isGrounded || collidesWithCollider<BubbleJumpableTopCollider>(registry, pos, collider)) {
                actor.isJumping = true;
                actor.jumpFrameCount = 0;
            }
        }

        // execute jump
        if (actor.isJumping) {
            actor.jumpFrameCount++;
            // vely = -2 * dragonJumpingSpeeds[actor.jumpFrameCount];
            vely = actor.jumpFrameCount < JUMP_SLOWDOWN_COUNT ? -jumpSpeed : -jumpEndSpeed;

            if (actor.jumpFrameCount == JUMP_FRAME_COUNT - 1) {
                actor.isJumping = false;
            }
        } else {
            vely = fallSpeed;
        }

        pos.x += velx;
        if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
            pos.x -= velx;
        }
        pos.x = std::max(2 * UNITS_PER_BLOCK, pos.x);
        pos.x = std::min(28 * UNITS_PER_BLOCK, pos.x);

        pos.y += vely;
        if (!actor.isJumping) {
            if (!actor.ignoreCollisions && collidesWithWall(registry, pos, collider)) {
                pos.y -= vely;
            }

            if (pos.y >= BOTTEM_WARP_POS) {
                pos.y -= TOP_WARP_POS;
            }
        }
    }
}
