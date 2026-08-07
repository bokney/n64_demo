#include "ecs.h"

Transform transforms[MAX_ENTITIES];
bool has_transform[MAX_ENTITIES];
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
            has_transform[i] = false;
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
    has_transform[e] = false;
    has_sprite[e] = false;
    has_text[e] = false;
    has_triangle[e] = false;
    has_input_mover[e] = false;
}

void ecs_add_transform(entity_t e, Transform t) {
    transforms[e] = t;
    has_transform[e] = true;
}

void ecs_add_sprite(entity_t e, Sprite t) {
    sprites[e] = t;
    has_sprite[e] = true;
}

void ecs_add_text(entity_t e, Text t) {
    texts[e] = t;
    has_text[e] = true;
}

void ecs_add_triangle(entity_t e, Triangle t) {
    triangles[e] = t;
    has_triangle[e] = true;
}

void ecs_add_input_mover(entity_t e, InputMover t) {
    input_movers[e] = t;
    has_input_mover[e] = true;
}

void ecs_remove_transform(entity_t e) {
    has_transform[e] = false;
}

void ecs_remove_sprite(entity_t e) {
    has_sprite[e] = false;
}

void ecs_remove_text(entity_t e) {
    has_text[e] = false;
}

void ecs_remove_triangle(entity_t e) {
    has_triangle[e] = false;
}

void ecs_remove_input_mover(entity_t e) {
    has_input_mover[e] = false;
}

bool ecs_has_transform(entity_t e) {
    return has_transform[e];
}

bool ecs_has_sprite(entity_t e) {
    return has_sprite[e];
}

bool ecs_has_text(entity_t e) {
    return has_text[e];
}

bool ecs_has_triangle(entity_t e) {
    return has_triangle[e];
}

bool ecs_has_input_mover(entity_t e) {
    return has_input_mover[e];
}

Transform *ecs_get_transform(entity_t e) {
    return &transforms[e];
}

Sprite *ecs_get_sprite(entity_t e) {
    return &sprites[e];
}

Text *ecs_get_text(entity_t e) {
    return &texts[e];
}

Triangle *ecs_get_triangle(entity_t e) {
    return &triangles[e];
}

InputMover *ecs_get_input_mover(entity_t e) {
    return &input_movers[e];
}

void ecs_tick_logic(void) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entity_alive[i] || !has_transform[i] || !has_input_mover[i]) {
            continue;
        }
        InputMover *mover = &input_movers[i];
        Transform *t = &transforms[i];
        if (input_action_held(mover->move_up)) {
            t->y -= mover->speed;
        }
        if (input_action_held(mover->move_down)) {
            t->y += mover->speed;
        }
        if (input_action_held(mover->move_left)) {
            t->x -= mover->speed;
        }
        if (input_action_held(mover->move_right)) {
            t->x += mover->speed;
        }
    }
}