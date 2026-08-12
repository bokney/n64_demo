#include "components.h"

Velocity velocities[MAX_ENTITIES];
bool has_velocity[MAX_ENTITIES];

CircleCollider colliders[MAX_ENTITIES];
bool has_collider[MAX_ENTITIES];

Health healths[MAX_ENTITIES];
bool has_health[MAX_ENTITIES];

Damage damages[MAX_ENTITIES];
bool has_damage[MAX_ENTITIES];

Team teams[MAX_ENTITIES];
bool has_team[MAX_ENTITIES];

Lifetime lifetimes[MAX_ENTITIES];
bool has_lifetime[MAX_ENTITIES];

OffscreenDespawn offscreen_despawns[MAX_ENTITIES];
bool has_offscreen_despawn[MAX_ENTITIES];

Enemy enemies[MAX_ENTITIES];
bool has_enemy[MAX_ENTITIES];

void gameplay_add_velocity(entity_t e, Velocity v) {
    velocities[e] = v;
    has_velocity[e] = true;
}

void gameplay_remove_velocity(entity_t e) {
    has_velocity[e] = false;
}

bool gameplay_has_velocity(entity_t e) {
    return has_velocity[e];
}

Velocity *gameplay_get_velocity(entity_t e) {
    return &velocities[e];
}

void gameplay_add_collider(entity_t e, CircleCollider c) {
    colliders[e] = c;
    has_collider[e] = true;
}

void gameplay_remove_collider(entity_t e) {
    has_collider[e] = false;
}

bool gameplay_has_collider(entity_t e) {
    return has_collider[e];
}

CircleCollider *gameplay_get_collider(entity_t e) {
    return &colliders[e];
}

void gameplay_add_health(entity_t e, Health h) {
    healths[e] = h;
    has_health[e] = true;
}

void gameplay_remove_health(entity_t e) {
    has_health[e] = false;
}

bool gameplay_has_health(entity_t e) {
    return has_health[e];
}

Health *gameplay_get_health(entity_t e) {
    return &healths[e];
}

void gameplay_add_damage(entity_t e, Damage d) {
    damages[e] = d;
    has_damage[e] = true;
}

void gameplay_remove_damage(entity_t e) {
    has_damage[e] = false;
}

bool gameplay_has_damage(entity_t e) {
    return has_damage[e];
}

Damage *gameplay_get_damage(entity_t e) {
    return &damages[e];
}

void gameplay_add_team(entity_t e, Team t) {
    teams[e] = t;
    has_team[e] = true;
}

void gameplay_remove_team(entity_t e) {
    has_team[e] = false;
}

bool gameplay_has_team(entity_t e) {
    return has_team[e];
}

Team *gameplay_get_team(entity_t e) {
    return &teams[e];
}

void gameplay_add_lifetime(entity_t e, Lifetime l) {
    lifetimes[e] = l;
    has_lifetime[e] = true;
}

void gameplay_remove_lifetime(entity_t e) {
    has_lifetime[e] = false;
}

bool gameplay_has_lifetime(entity_t e) {
    return has_lifetime[e];
}

Lifetime *gameplay_get_lifetime(entity_t e) {
    return &lifetimes[e];
}

void gameplay_add_offscreen_despawn(entity_t e, OffscreenDespawn o) {
    offscreen_despawns[e] = o;
    has_offscreen_despawn[e] = true;
}

void gameplay_remove_offscreen_despawn(entity_t e) {
    has_offscreen_despawn[e] = false;
}

bool gameplay_has_offscreen_despawn(entity_t e) {
    return has_offscreen_despawn[e];
}

OffscreenDespawn *gameplay_get_offscreen_despawn(entity_t e) {
    return &offscreen_despawns[e];
}

void gameplay_add_enemy(entity_t e, Enemy en) {
    enemies[e] = en;
    has_enemy[e] = true;
}

void gameplay_remove_enemy(entity_t e) {
    has_enemy[e] = false;
}

bool gameplay_has_enemy(entity_t e) {
    return has_enemy[e];
}

Enemy *gameplay_get_enemy(entity_t e) {
    return &enemies[e];
}