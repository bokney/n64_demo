#include "camera_components.h"

Camera cameras[MAX_ENTITIES];
bool has_camera[MAX_ENTITIES];
CameraBehavior camera_behaviors[MAX_ENTITIES];
bool has_camera_behavior[MAX_ENTITIES];

void ecs_add_camera(entity_t e, Camera c) {
    if (e >= MAX_ENTITIES) return;
    cameras[e] = c;
    has_camera[e] = true;
}

void ecs_remove_camera(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_camera[e] = false;
}

bool ecs_has_camera(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_camera[e];
}

Camera *ecs_get_camera(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &cameras[e];
}

void ecs_add_camera_behavior(entity_t e, CameraBehavior cb) {
    if (e >= MAX_ENTITIES) return;
    camera_behaviors[e] = cb;
    has_camera_behavior[e] = true;
}

void ecs_remove_camera_behavior(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_camera_behavior[e] = false;
}

bool ecs_has_camera_behavior(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_camera_behavior[e];
}

CameraBehavior *ecs_get_camera_behavior(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &camera_behaviors[e];
}
