#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <libdragon.h>
#include "systems/input.h"

#define MAX_ENTITIES 64

typedef uint16_t entity_t;

typedef struct { float x, y, z; } Position;
typedef struct { float pitch, yaw, roll; } Rotation;
typedef struct { float x, y, z; } Scale;
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

typedef struct { uint16_t w, h; color_t color; } Sprite;
typedef struct { const char *str; int font_id; color_t color; } Text;
typedef struct { float v1x, v1y, v2x, v2y, v3x, v3y; color_t color; } Triangle;
typedef struct { int16_t speed; InputAction move_up, move_down, move_left, move_right; } InputMover;

extern Position positions[MAX_ENTITIES];
extern bool has_position[MAX_ENTITIES];
extern Rotation rotations[MAX_ENTITIES];
extern bool has_rotation[MAX_ENTITIES];
extern Scale scales[MAX_ENTITIES];
extern bool has_scale[MAX_ENTITIES];
extern Camera cameras[MAX_ENTITIES];
extern bool has_camera[MAX_ENTITIES];
extern CameraBehavior camera_behaviors[MAX_ENTITIES];
extern bool has_camera_behavior[MAX_ENTITIES];

extern Sprite sprites[MAX_ENTITIES];
extern bool has_sprite[MAX_ENTITIES];
extern Text texts[MAX_ENTITIES];
extern bool has_text[MAX_ENTITIES];
extern Triangle triangles[MAX_ENTITIES];
extern bool has_triangle[MAX_ENTITIES];
extern InputMover input_movers[MAX_ENTITIES];
extern bool has_input_mover[MAX_ENTITIES];
extern bool entity_alive[MAX_ENTITIES];

void ecs_add_position(entity_t e, Position p);
void ecs_remove_position(entity_t e);
bool ecs_has_position(entity_t e);
Position *ecs_get_position(entity_t e);

void ecs_add_rotation(entity_t e, Rotation r);
void ecs_remove_rotation(entity_t e);
bool ecs_has_rotation(entity_t e);
Rotation *ecs_get_rotation(entity_t e);

void ecs_add_scale(entity_t e, Scale s);
void ecs_remove_scale(entity_t e);
bool ecs_has_scale(entity_t e);
Scale *ecs_get_scale(entity_t e);

void ecs_add_camera(entity_t e, Camera c);
void ecs_remove_camera(entity_t e);
bool ecs_has_camera(entity_t e);
Camera *ecs_get_camera(entity_t e);

void ecs_add_camera_behavior(entity_t e, CameraBehavior cb);
void ecs_remove_camera_behavior(entity_t e);
bool ecs_has_camera_behavior(entity_t e);
CameraBehavior *ecs_get_camera_behavior(entity_t e);

void ecs_add_sprite(entity_t e, Sprite s);
void ecs_remove_sprite(entity_t e);
bool ecs_has_sprite(entity_t e);
Sprite *ecs_get_sprite(entity_t e);

void ecs_add_text(entity_t e, Text t);
void ecs_remove_text(entity_t e);
bool ecs_has_text(entity_t e);
Text *ecs_get_text(entity_t e);

void ecs_add_triangle(entity_t e, Triangle t);
void ecs_remove_triangle(entity_t e);
bool ecs_has_triangle(entity_t e);
Triangle *ecs_get_triangle(entity_t e);

void ecs_add_input_mover(entity_t e, InputMover m);
void ecs_remove_input_mover(entity_t e);
bool ecs_has_input_mover(entity_t e);
InputMover *ecs_get_input_mover(entity_t e);

#endif