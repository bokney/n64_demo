#include "ecs.h"

Position positions[MAX_ENTITIES];
bool has_position[MAX_ENTITIES];
Rotation rotations[MAX_ENTITIES];
bool has_rotation[MAX_ENTITIES];
Scale scales[MAX_ENTITIES];
bool has_scale[MAX_ENTITIES];
bool entity_alive[MAX_ENTITIES];

entity_t ecs_create_entity(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entity_alive[i]) {
            entity_alive[i] = true;
            has_position[i] = false;
            has_rotation[i] = false;
            has_scale[i] = false;
            has_camera[i] = false;
            has_camera_behavior[i] = false;
            has_sprite[i] = false;
            has_text[i] = false;
            has_triangle[i] = false;
            has_input_mover[i] = false;
            return i;
        }
    }
    return MAX_ENTITIES;
}

void ecs_destroy_entity(entity_t e) {
    entity_alive[e] = false;
    has_position[e] = false;
    has_rotation[e] = false;
    has_scale[e] = false;
    has_camera[e] = false;
    has_camera_behavior[e] = false;
    has_sprite[e] = false;
    has_text[e] = false;
    has_triangle[e] = false;
    has_mesh[e] = false;
            has_input_mover[e] = false;
    has_lighting[e] = false;
            has_lighting[e] = false;
}

void ecs_add_position(entity_t e, Position p) {
    positions[e] = p;
    has_position[e] = true;
}

void ecs_remove_position(entity_t e) {
    has_position[e] = false;
}

bool ecs_has_position(entity_t e) {
    return has_position[e];
}

Position *ecs_get_position(entity_t e) {
    return &positions[e];
}

void ecs_add_rotation(entity_t e, Rotation r) {
    rotations[e] = r;
    has_rotation[e] = true;
}

void ecs_remove_rotation(entity_t e) {
    has_rotation[e] = false;
}

bool ecs_has_rotation(entity_t e) {
    return has_rotation[e];
}

Rotation *ecs_get_rotation(entity_t e) {
    return &rotations[e];
}

void ecs_add_scale(entity_t e, Scale s) {
    scales[e] = s;
    has_scale[e] = true;
}

void ecs_remove_scale(entity_t e) {
    has_scale[e] = false;
}

bool ecs_has_scale(entity_t e) {
    return has_scale[e];
}

Scale *ecs_get_scale(entity_t e) {
    return &scales[e];
}

void ecs_tick_logic(input_action_held_t input_action_held) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entity_alive[i] || !has_position[i] || !has_input_mover[i]) {
            continue;
        }
        InputMover *mover = &input_movers[i];
        Position *p = &positions[i];
        if (input_action_held(ACTION_UP)) {
            p->y -= mover->speed;
        }
        if (input_action_held(ACTION_DOWN)) {
            p->y += mover->speed;
        }
        if (input_action_held(ACTION_LEFT)) {
            p->x -= mover->speed;
        }
        if (input_action_held(ACTION_RIGHT)) {
            p->x += mover->speed;
        }
    }
}
