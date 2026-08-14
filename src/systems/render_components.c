#include "render_components.h"

Sprite sprites[MAX_ENTITIES];
bool has_sprite[MAX_ENTITIES];
Text texts[MAX_ENTITIES];
bool has_text[MAX_ENTITIES];
Triangle triangles[MAX_ENTITIES];
bool has_triangle[MAX_ENTITIES];
Mesh meshes[MAX_ENTITIES];
bool has_mesh[MAX_ENTITIES];

void ecs_add_sprite(entity_t e, Sprite s) {
    if (e >= MAX_ENTITIES) return;
    sprites[e] = s;
    has_sprite[e] = true;
}

void ecs_remove_sprite(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_sprite[e] = false;
}

bool ecs_has_sprite(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_sprite[e];
}

Sprite *ecs_get_sprite(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &sprites[e];
}

void ecs_add_text(entity_t e, Text t) {
    if (e >= MAX_ENTITIES) return;
    texts[e] = t;
    has_text[e] = true;
}

void ecs_remove_text(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_text[e] = false;
}

bool ecs_has_text(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_text[e];
}

Text *ecs_get_text(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &texts[e];
}

void ecs_add_triangle(entity_t e, Triangle t) {
    if (e >= MAX_ENTITIES) return;
    triangles[e] = t;
    has_triangle[e] = true;
}

void ecs_remove_triangle(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_triangle[e] = false;
}

bool ecs_has_triangle(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_triangle[e];
}

Triangle *ecs_get_triangle(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &triangles[e];
}

void ecs_add_mesh(entity_t e, Mesh m) {
    if (e >= MAX_ENTITIES) return;
    meshes[e] = m;
    has_mesh[e] = true;
}

void ecs_remove_mesh(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_mesh[e] = false;
}

bool ecs_has_mesh(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_mesh[e];
}

Mesh *ecs_get_mesh(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &meshes[e];
}
