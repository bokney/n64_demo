#include "transform_components.h"

Position positions[MAX_ENTITIES];
bool has_position[MAX_ENTITIES];
Rotation rotations[MAX_ENTITIES];
bool has_rotation[MAX_ENTITIES];
Scale scales[MAX_ENTITIES];
bool has_scale[MAX_ENTITIES];

void ecs_add_position(entity_t e, Position p) {
    if (e >= MAX_ENTITIES) return;
    positions[e] = p;
    has_position[e] = true;
}

void ecs_remove_position(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_position[e] = false;
}

bool ecs_has_position(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_position[e];
}

Position *ecs_get_position(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &positions[e];
}

void ecs_add_rotation(entity_t e, Rotation r) {
    if (e >= MAX_ENTITIES) return;
    rotations[e] = r;
    has_rotation[e] = true;
}

void ecs_remove_rotation(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_rotation[e] = false;
}

bool ecs_has_rotation(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_rotation[e];
}

Rotation *ecs_get_rotation(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &rotations[e];
}

void ecs_add_scale(entity_t e, Scale s) {
    if (e >= MAX_ENTITIES) return;
    scales[e] = s;
    has_scale[e] = true;
}

void ecs_remove_scale(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_scale[e] = false;
}

bool ecs_has_scale(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_scale[e];
}

Scale *ecs_get_scale(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &scales[e];
}
