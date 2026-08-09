#include "ecs.h"

Position positions[MAX_ENTITIES];
bool has_position[MAX_ENTITIES];
Rotation rotations[MAX_ENTITIES];
bool has_rotation[MAX_ENTITIES];
Scale scales[MAX_ENTITIES];
bool has_scale[MAX_ENTITIES];
Camera cameras[MAX_ENTITIES];
bool has_camera[MAX_ENTITIES];
CameraBehavior camera_behaviors[MAX_ENTITIES];
bool has_camera_behavior[MAX_ENTITIES];

Sprite sprites[MAX_ENTITIES];
bool has_sprite[MAX_ENTITIES];
Text texts[MAX_ENTITIES];
bool has_text[MAX_ENTITIES];
Triangle triangles[MAX_ENTITIES];
bool has_triangle[MAX_ENTITIES];
InputMover input_movers[MAX_ENTITIES];
bool has_input_mover[MAX_ENTITIES];
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
    has_input_mover[e] = false;
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

void ecs_add_camera(entity_t e, Camera c) {
    cameras[e] = c;
    has_camera[e] = true;
}

void ecs_remove_camera(entity_t e) {
    has_camera[e] = false;
}

bool ecs_has_camera(entity_t e) {
    return has_camera[e];
}

Camera *ecs_get_camera(entity_t e) {
    return &cameras[e];
}

void ecs_add_camera_behavior(entity_t e, CameraBehavior cb) {
    camera_behaviors[e] = cb;
    has_camera_behavior[e] = true;
}

void ecs_remove_camera_behavior(entity_t e) {
    has_camera_behavior[e] = false;
}

bool ecs_has_camera_behavior(entity_t e) {
    return has_camera_behavior[e];
}

CameraBehavior *ecs_get_camera_behavior(entity_t e) {
    return &camera_behaviors[e];
}

void ecs_add_sprite(entity_t e, Sprite s) {
    sprites[e] = s;
    has_sprite[e] = true;
}

void ecs_remove_sprite(entity_t e) {
    has_sprite[e] = false;
}

bool ecs_has_sprite(entity_t e) {
    return has_sprite[e];
}

Sprite *ecs_get_sprite(entity_t e) {
    return &sprites[e];
}

void ecs_add_text(entity_t e, Text t) {
    texts[e] = t;
    has_text[e] = true;
}

void ecs_remove_text(entity_t e) {
    has_text[e] = false;
}

bool ecs_has_text(entity_t e) {
    return has_text[e];
}

Text *ecs_get_text(entity_t e) {
    return &texts[e];
}

void ecs_add_triangle(entity_t e, Triangle t) {
    triangles[e] = t;
    has_triangle[e] = true;
}

void ecs_remove_triangle(entity_t e) {
    has_triangle[e] = false;
}

bool ecs_has_triangle(entity_t e) {
    return has_triangle[e];
}

Triangle *ecs_get_triangle(entity_t e) {
    return &triangles[e];
}

void ecs_add_input_mover(entity_t e, InputMover m) {
    input_movers[e] = m;
    has_input_mover[e] = true;
}

void ecs_remove_input_mover(entity_t e) {
    has_input_mover[e] = false;
}

bool ecs_has_input_mover(entity_t e) {
    return has_input_mover[e];
}

InputMover *ecs_get_input_mover(entity_t e) {
    return &input_movers[e];
}

void ecs_tick_logic(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entity_alive[i] || !has_position[i] || !has_input_mover[i]) {
            continue;
        }
        InputMover *mover = &input_movers[i];
        Position *p = &positions[i];
        if (input_action_held(mover->move_up)) {
            p->y -= mover->speed;
        }
        if (input_action_held(mover->move_down)) {
            p->y += mover->speed;
        }
        if (input_action_held(mover->move_left)) {
            p->x -= mover->speed;
        }
        if (input_action_held(mover->move_right)) {
            p->x += mover->speed;
        }
    }
}