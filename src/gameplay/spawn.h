#ifndef GAMEPLAY_SPAWN_H
#define GAMEPLAY_SPAWN_H

#include <stdint.h>
#include "../ecs_components.h"
#include "components.h"

typedef struct {
    float trigger_scroll_z;
    EnemyType enemy_type;
    float x_pos;
    union {
        struct { float speed_z; } straight;
        struct { float speed_z; float amplitude; float frequency; } sine;
    };
} SpawnEntry;

extern const SpawnEntry spawn_table[];
extern const int spawn_table_count;
extern int next_spawn_index;

void spawn_system(float current_scroll_z);
entity_t spawn_enemy(const SpawnEntry *entry);

#endif