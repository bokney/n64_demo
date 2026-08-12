#include "gameplay.h"
#include "../game.h"
#include "../ecs.h"
#include "../systems/input.h"
#include "../systems/camera.h"
#include "../gameplay/components.h"
#include "../gameplay/spawn.h"

#define FB_COUNT 3

#define CAM_HEIGHT 100.0f
#define PLAYER_SPEED 3.0f

#define DESPAWN_TOP    -96.0f
#define DESPAWN_BOTTOM 336.0f
#define DESPAWN_LEFT  -96.0f
#define DESPAWN_RIGHT  416.0f

static entity_t player_entity = MAX_ENTITIES;

static void system_move(void) {
    const float dt = 1.0f / 60.0f;
    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (!entity_alive[e] || !has_velocity[e] || !has_position[e]) continue;
        Velocity *vel = gameplay_get_velocity(e);
        Position *pos = ecs_get_position(e);
        pos->x += vel->x * dt;
        pos->y += vel->y * dt;
        pos->z += vel->z * dt;

        if (has_enemy[e]) {
            Enemy *enemy = gameplay_get_enemy(e);
            if (enemy->type == ENEMY_TYPE_SINE) {
                pos->x = enemy->sine.spawn_x + sinf(pos->y * enemy->sine.frequency) * enemy->sine.amplitude;
            }
        }
    }
}

static void system_offscreen_despawn(void) {
    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (!entity_alive[e] || !has_offscreen_despawn[e] || !has_position[e]) continue;
        Position *pos = ecs_get_position(e);
        if (pos->y > DESPAWN_BOTTOM || pos->y < DESPAWN_TOP || 
            pos->x < DESPAWN_LEFT || pos->x > DESPAWN_RIGHT) {
            ecs_destroy_entity(e);
        }
    }
}

void gameplay_init(void) {
    // Player entity (2D sprite)
    player_entity = ecs_create_entity();
    ecs_add_position(player_entity, (Position){160.0f, 180.0f, LAYER_PLAYER});
    ecs_add_sprite(player_entity, (Sprite){24, 24, RGBA32(0x00, 0xff, 0x88, 0xff)});
    ecs_add_input_mover(player_entity, (InputMover){
        .speed = PLAYER_SPEED,
        .move_up = ACTION_UP,
        .move_down = ACTION_DOWN,
        .move_left = ACTION_LEFT,
        .move_right = ACTION_RIGHT
    });
    gameplay_add_collider(player_entity, (CircleCollider){ .radius = 12.0f });
    gameplay_add_health(player_entity, (Health){ .current = 3, .max = 3 });
    gameplay_add_team(player_entity, (Team){ .team = TEAM_PLAYER });

    next_spawn_index = 0;
}

uint8_t gameplay_update(void) {
    if (input_action_pressed(ACTION_CANCEL)) {
        return STATE_TITLE_SCREEN;
    }

    system_move();
    spawn_system(0.0f);
    system_offscreen_despawn();

    return 0;
}

uint8_t gameplay_exit(void) {
    if (player_entity != MAX_ENTITIES) {
        ecs_destroy_entity(player_entity);
        player_entity = MAX_ENTITIES;
    }

    // Destroy all gameplay-owned entities (enemies, bullets, etc.)
    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (entity_alive[e] && has_team[e] && 
            (teams[e].team == TEAM_PLAYER || teams[e].team == TEAM_ENEMY)) {
            ecs_destroy_entity(e);
        }
    }
    next_spawn_index = 0;

    return 0;
}
