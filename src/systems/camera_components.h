#ifndef CAMERA_COMPONENTS_H
#define CAMERA_COMPONENTS_H

#include "../ecs_components.h"
#include <libdragon.h>

typedef struct {
    fm_vec3_t forward;
    fm_vec3_t up;
    float fov;
    float near, far;
    float left, right, top, bottom;
    bool is_ortho;
    bool is_active;
} Camera;

typedef enum {
    CAMERA_BEHAVIOR_NONE = 0,
    CAMERA_BEHAVIOR_ORBIT,
    CAMERA_BEHAVIOR_FOLLOW,
    CAMERA_BEHAVIOR_STATIC,
    CAMERA_BEHAVIOR_SCROLL
} CameraBehaviorType;

typedef struct {
    CameraBehaviorType type;
    union {
        struct { entity_t target; float distance; float yaw; float pitch; float min_pitch, max_pitch; } orbit;
        struct { entity_t target; fm_vec3_t offset; float lerp_speed; } follow;
        struct { } static_cam;
        struct { float scroll_speed_x, scroll_speed_z; float current_offset_x, current_offset_z; } scroll;
    };
} CameraBehavior;

extern Camera cameras[MAX_ENTITIES];
extern bool has_camera[MAX_ENTITIES];
extern CameraBehavior camera_behaviors[MAX_ENTITIES];
extern bool has_camera_behavior[MAX_ENTITIES];

void ecs_add_camera(entity_t e, Camera c);
void ecs_remove_camera(entity_t e);
bool ecs_has_camera(entity_t e);
Camera *ecs_get_camera(entity_t e);

void ecs_add_camera_behavior(entity_t e, CameraBehavior cb);
void ecs_remove_camera_behavior(entity_t e);
bool ecs_has_camera_behavior(entity_t e);
CameraBehavior *ecs_get_camera_behavior(entity_t e);

#endif
