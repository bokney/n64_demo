#include "main_menu.h"
#include "../ecs.h"
#include "../systems/input.h"
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

#define FB_COUNT 3

#define MODEL_SCALE 0.1f

static T3DModel *cube_model = NULL;
static T3DMat4FP *model_mats = NULL;
static int frame_idx = 0;

static const uint8_t color_ambient[4] = {80, 80, 100, 0xFF};
static const uint8_t color_dir[4]     = {0xEE, 0xAA, 0xAA, 0xFF};

#define CAM_TURN_SPEED   0.06f
#define CAM_STRAFE_SPEED 1.5f
#define CAM_VERT_SPEED   1.5f
#define CAM_PITCH_MIN    T3D_DEG_TO_RAD(-85.0f)
#define CAM_PITCH_MAX    T3D_DEG_TO_RAD(85.0f)

static entity_t cam_entity = MAX_ENTITIES;
static entity_t cam_target_entity = MAX_ENTITIES;

void main_menu_init(void) {
    cube_model = t3d_model_load("rom:/models/colourful_cube.t3dm");

    cam_target_entity = ecs_create_entity();
    ecs_add_position(cam_target_entity, (Position){0.0f, 0.0f, 0.0f});

    cam_entity = ecs_create_entity();
    ecs_add_position(cam_entity, (Position){0.0f, 12.0f, 25.0f});
    ecs_add_camera(cam_entity, (Camera){
        .forward = {{0.0f, -0.447f, -0.894f}},
        .up = {{0.0f, 1.0f, 0.0f}},
        .fov = T3D_DEG_TO_RAD(60.0f),
        .near = 10.0f,
        .far = 150.0f,
        .is_ortho = false,
        .is_active = true
    });
    ecs_add_camera_behavior(cam_entity, (CameraBehavior){
        .type = CAMERA_BEHAVIOR_ORBIT,
        .orbit = {
            .target = cam_target_entity,
            .distance = 25.0f,
            .yaw = 0.0f,
            .pitch = -0.44f,
            .min_pitch = CAM_PITCH_MIN,
            .max_pitch = CAM_PITCH_MAX
        }
    });

    model_mats = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    frame_idx = 0;
}

uint8_t main_menu_update(void) {
    CameraBehavior *cb = ecs_get_camera_behavior(cam_entity);
    if (!cb) return 0;

    bool turning = input_action_held(ACTION_C_LEFT)  || input_action_held(ACTION_C_RIGHT) ||
                   input_action_held(ACTION_C_UP)    || input_action_held(ACTION_C_DOWN);
    if (turning) {
        if (input_action_held(ACTION_C_LEFT))  cb->orbit.yaw   -= CAM_TURN_SPEED;
        if (input_action_held(ACTION_C_RIGHT)) cb->orbit.yaw   += CAM_TURN_SPEED;
        if (input_action_held(ACTION_C_UP))    cb->orbit.pitch += CAM_TURN_SPEED;
        if (input_action_held(ACTION_C_DOWN))  cb->orbit.pitch -= CAM_TURN_SPEED;

        if (cb->orbit.pitch > cb->orbit.max_pitch) cb->orbit.pitch = cb->orbit.max_pitch;
        if (cb->orbit.pitch < cb->orbit.min_pitch) cb->orbit.pitch = cb->orbit.min_pitch;
    }

    if (input_action_held(ACTION_CONFIRM)) {
        Position *target_pos = ecs_get_position(cam_target_entity);
        if (target_pos) {
            float yaw = cb->orbit.yaw;
            float cp = fm_cosf(cb->orbit.pitch);
            fm_vec3_t forward = {
                .v = {cp * fm_sinf(yaw), fm_sinf(cb->orbit.pitch), cp * fm_cosf(yaw)}
            };
            fm_vec3_norm(&forward, &forward);

            fm_vec3_t right = {
                .v = {forward.v[2], 0.0f, -forward.v[0]}
            };
            fm_vec3_norm(&right, &right);

            if (input_action_held(ACTION_LEFT)) {
                target_pos->x -= right.v[0] * CAM_STRAFE_SPEED;
                target_pos->z -= right.v[2] * CAM_STRAFE_SPEED;
            }
            if (input_action_held(ACTION_RIGHT)) {
                target_pos->x += right.v[0] * CAM_STRAFE_SPEED;
                target_pos->z += right.v[2] * CAM_STRAFE_SPEED;
            }
        }
    }

    if (input_action_held(ACTION_CANCEL)) {
        Position *target_pos = ecs_get_position(cam_target_entity);
        if (target_pos) {
            if (input_action_held(ACTION_UP)) {
                target_pos->y += CAM_VERT_SPEED;
            }
            if (input_action_held(ACTION_DOWN)) {
                target_pos->y -= CAM_VERT_SPEED;
            }
        }
    }

    return 0;
}

void main_menu_render_3d(T3DViewport *viewport) {
    if (cube_model == NULL) return;

    frame_idx = (frame_idx + 1) % FB_COUNT;

    t3d_mat4fp_from_srt_euler(&model_mats[frame_idx],
        (float[3]){MODEL_SCALE, MODEL_SCALE, MODEL_SCALE},
        (float[3]){0.0f, 0.0f, 0.0f},
        (float[3]){0.0f, 0.0f, 0.0f}
    );

    t3d_frame_start();
    t3d_viewport_attach(viewport);

    t3d_screen_clear_color(RGBA32(0x3f, 0x3f, 0x74, 0xff));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(color_ambient);
    fm_vec3_t ldir = {{-1.0f, 1.0f, 1.0f}};
    fm_vec3_norm(&ldir, &ldir);
    t3d_light_set_directional(0, color_dir, &ldir);
    t3d_light_set_count(1);

    t3d_matrix_push(&model_mats[frame_idx]);
    t3d_model_draw(cube_model);
    t3d_matrix_pop(1);
}

uint8_t main_menu_exit(void) {
    if (cube_model) {
        t3d_model_free(cube_model);
        cube_model = NULL;
    }
    if (model_mats) {
        free_uncached(model_mats);
        model_mats = NULL;
    }
    if (cam_entity != MAX_ENTITIES) {
        ecs_destroy_entity(cam_entity);
        cam_entity = MAX_ENTITIES;
    }
    if (cam_target_entity != MAX_ENTITIES) {
        ecs_destroy_entity(cam_target_entity);
        cam_target_entity = MAX_ENTITIES;
    }
    return 0;
}
