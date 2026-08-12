#include "spawn.h"
#include "../ecs.h"
#include "../systems/camera.h"

const SpawnEntry spawn_table[] = {
    { 50.0f,  ENEMY_TYPE_STRAIGHT, -80.0f,  .straight = { .speed_z = 240.0f } },
    { 50.0f,  ENEMY_TYPE_STRAIGHT,  80.0f,  .straight = { .speed_z = 240.0f } },
    { 120.0f, ENEMY_TYPE_STRAIGHT,  0.0f,   .straight = { .speed_z = 300.0f } },
    { 200.0f, ENEMY_TYPE_STRAIGHT, -100.0f, .straight = { .speed_z = 270.0f } },
    { 200.0f, ENEMY_TYPE_STRAIGHT,  100.0f, .straight = { .speed_z = 270.0f } },
    { 300.0f, ENEMY_TYPE_SINE,      0.0f,   .sine = { .speed_z = 240.0f, .amplitude = 15.0f, .frequency = 0.05f } },
    { 400.0f, ENEMY_TYPE_STRAIGHT, -120.0f, .straight = { .speed_z = 300.0f } },
    { 400.0f, ENEMY_TYPE_STRAIGHT,  120.0f, .straight = { .speed_z = 300.0f } },
    { 500.0f, ENEMY_TYPE_STRAIGHT,  0.0f,   .straight = { .speed_z = 360.0f } },
};

const int spawn_table_count = sizeof(spawn_table) / sizeof(spawn_table[0]);

int next_spawn_index = 0;

void spawn_system(float current_scroll_z) {
    while (next_spawn_index < spawn_table_count) {
        const SpawnEntry *entry = &spawn_table[next_spawn_index];
        if (entry->trigger_scroll_z > current_scroll_z) {
            break;
        }
        spawn_enemy(entry);
        next_spawn_index++;
    }
}

entity_t spawn_enemy(const SpawnEntry *entry) {
    entity_t e = ecs_create_entity();
    if (e == MAX_ENTITIES) return MAX_ENTITIES;

    ecs_add_position(e, (Position){ entry->x_pos, -32.0f, LAYER_ENEMY });
    ecs_add_scale(e, (Scale){ 1.0f, 1.0f, 1.0f });

    gameplay_add_collider(e, (CircleCollider){ .radius = 16.0f });
    gameplay_add_health(e, (Health){ .current = 3, .max = 3 });
    gameplay_add_team(e, (Team){ .team = TEAM_ENEMY });
    gameplay_add_offscreen_despawn(e, (OffscreenDespawn){ .enabled = true });

    Velocity vel = { 0.0f, 0.0f, 0.0f };
    Enemy enemy = { .type = entry->enemy_type };

    switch (entry->enemy_type) {
        case ENEMY_TYPE_STRAIGHT:
            vel.y = entry->straight.speed_z;
            enemy.straight.speed_z = entry->straight.speed_z;
            break;
        case ENEMY_TYPE_SINE:
            vel.y = entry->sine.speed_z;
            enemy.sine.speed_z = entry->sine.speed_z;
            enemy.sine.amplitude = entry->sine.amplitude;
            enemy.sine.frequency = entry->sine.frequency;
            enemy.sine.spawn_x = entry->x_pos;
            break;
    }
    gameplay_add_velocity(e, vel);
    gameplay_add_enemy(e, enemy);

    ecs_add_sprite(e, (Sprite){ 32, 32, RGBA32(0xff, 0x44, 0x44, 0xff) });

    return e;
}