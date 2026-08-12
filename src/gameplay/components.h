#ifndef GAMEPLAY_COMPONENTS_H
#define GAMEPLAY_COMPONENTS_H

#include <stdint.h>
#include <stdbool.h>
#include "../ecs_components.h"

typedef struct { float x, y, z; } Velocity;

typedef struct { float radius; } CircleCollider;

typedef struct { int current, max; } Health;

typedef struct { int amount; } Damage;

typedef enum {
    TEAM_PLAYER = 0,
    TEAM_ENEMY = 1,
    TEAM_NEUTRAL = 2,
} TeamType;

typedef struct { TeamType team; } Team;

typedef struct { float remaining; } Lifetime;

typedef struct { bool enabled; } OffscreenDespawn;

typedef enum {
    ENEMY_TYPE_STRAIGHT = 0,
    ENEMY_TYPE_SINE = 1,
} EnemyType;

typedef struct {
    EnemyType type;
    union {
        struct { float speed_z; } straight;
        struct { float speed_z; float amplitude; float frequency; float spawn_x; } sine;
    };
} Enemy;

#define LAYER_BG      0.0f
#define LAYER_ENEMY   1.0f
#define LAYER_BULLET  1.5f
#define LAYER_PLAYER  2.0f
#define LAYER_HUD    10.0f

extern Velocity velocities[MAX_ENTITIES];
extern bool has_velocity[MAX_ENTITIES];

extern CircleCollider colliders[MAX_ENTITIES];
extern bool has_collider[MAX_ENTITIES];

extern Health healths[MAX_ENTITIES];
extern bool has_health[MAX_ENTITIES];

extern Damage damages[MAX_ENTITIES];
extern bool has_damage[MAX_ENTITIES];

extern Team teams[MAX_ENTITIES];
extern bool has_team[MAX_ENTITIES];

extern Lifetime lifetimes[MAX_ENTITIES];
extern bool has_lifetime[MAX_ENTITIES];

extern OffscreenDespawn offscreen_despawns[MAX_ENTITIES];
extern bool has_offscreen_despawn[MAX_ENTITIES];

extern Enemy enemies[MAX_ENTITIES];
extern bool has_enemy[MAX_ENTITIES];

void gameplay_add_velocity(entity_t e, Velocity v);
void gameplay_remove_velocity(entity_t e);
bool gameplay_has_velocity(entity_t e);
Velocity *gameplay_get_velocity(entity_t e);

void gameplay_add_collider(entity_t e, CircleCollider c);
void gameplay_remove_collider(entity_t e);
bool gameplay_has_collider(entity_t e);
CircleCollider *gameplay_get_collider(entity_t e);

void gameplay_add_health(entity_t e, Health h);
void gameplay_remove_health(entity_t e);
bool gameplay_has_health(entity_t e);
Health *gameplay_get_health(entity_t e);

void gameplay_add_damage(entity_t e, Damage d);
void gameplay_remove_damage(entity_t e);
bool gameplay_has_damage(entity_t e);
Damage *gameplay_get_damage(entity_t e);

void gameplay_add_team(entity_t e, Team t);
void gameplay_remove_team(entity_t e);
bool gameplay_has_team(entity_t e);
Team *gameplay_get_team(entity_t e);

void gameplay_add_lifetime(entity_t e, Lifetime l);
void gameplay_remove_lifetime(entity_t e);
bool gameplay_has_lifetime(entity_t e);
Lifetime *gameplay_get_lifetime(entity_t e);

void gameplay_add_offscreen_despawn(entity_t e, OffscreenDespawn o);
void gameplay_remove_offscreen_despawn(entity_t e);
bool gameplay_has_offscreen_despawn(entity_t e);
OffscreenDespawn *gameplay_get_offscreen_despawn(entity_t e);

void gameplay_add_enemy(entity_t e, Enemy en);
void gameplay_remove_enemy(entity_t e);
bool gameplay_has_enemy(entity_t e);
Enemy *gameplay_get_enemy(entity_t e);

#endif