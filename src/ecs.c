#include "ecs.h"
#include "systems/transform_components.h"

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
            has_mesh[i] = false;
            has_input_mover[i] = false;
            has_lighting[i] = false;
            return i;
        }
    }
    return MAX_ENTITIES;
}

void ecs_destroy_entity(entity_t e) {
    if (e >= MAX_ENTITIES) return;
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
