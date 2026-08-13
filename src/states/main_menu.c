#include "main_menu.h"
#include "../ecs.h"
#include "../systems/input.h"
#include "../game.h"
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

#define FB_COUNT 3

#define MODEL_SCALE 0.1f
#define MODEL_ROT_SPEED  0.03f
#define CAM_FOV_SPEED    1.0f
#define CAM_FOV_MIN      20.0f
#define CAM_FOV_MAX      90.0f

static T3DModel *cube_model = NULL;
static T3DMat4FP *model_mats = NULL;
static int frame_idx = 0;

static const uint8_t color_ambient[4] = {80, 80, 100, 0xFF};
static const uint8_t color_dir[4]     = {0xEE, 0xAA, 0xAA, 0xFF};

static float model_yaw = 0.0f;
static float model_pitch = 0.0f;
static float model_roll = 0.0f;
static entity_t cam_entity = MAX_ENTITIES;

void main_menu_init(void) {
    cube_model = t3d_model_load("rom:/models/colourful_cube.t3dm");

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

    model_mats = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    frame_idx = 0;
}

uint8_t main_menu_update(void) {
    Camera *cam = ecs_get_camera(cam_entity);
    if (!cam) return 0;

    // Model rotation via C-buttons
    if (input_action_held(ACTION_C_LEFT))  model_yaw   += MODEL_ROT_SPEED;
    if (input_action_held(ACTION_C_RIGHT)) model_yaw   -= MODEL_ROT_SPEED;
    if (input_action_held(ACTION_C_UP))    model_pitch += MODEL_ROT_SPEED;
    if (input_action_held(ACTION_C_DOWN))  model_pitch -= MODEL_ROT_SPEED;

    // Model roll via shoulder buttons
    if (input_action_held(ACTION_L)) model_roll -= MODEL_ROT_SPEED;
    if (input_action_held(ACTION_R)) model_roll += MODEL_ROT_SPEED;

    // Camera FOV zoom via control stick up/down (Lakitu style)
    if (input_action_held(ACTION_UP)) {
        cam->fov -= CAM_FOV_SPEED * T3D_DEG_TO_RAD(1.0f);
        if (cam->fov < T3D_DEG_TO_RAD(CAM_FOV_MIN)) cam->fov = T3D_DEG_TO_RAD(CAM_FOV_MIN);
    }
    if (input_action_held(ACTION_DOWN)) {
        cam->fov += CAM_FOV_SPEED * T3D_DEG_TO_RAD(1.0f);
        if (cam->fov > T3D_DEG_TO_RAD(CAM_FOV_MAX)) cam->fov = T3D_DEG_TO_RAD(CAM_FOV_MAX);
    }

    // Reset model rotation
    if (input_action_pressed(ACTION_CONFIRM)) {
        model_yaw = model_pitch = model_roll = 0.0f;
    }

    // Start gameplay
    if (input_action_pressed(ACTION_PAUSE)) {
        return STATE_GAMEPLAY;
    }

    return 0;
}

void main_menu_render(T3DViewport *viewport) {
    if (cube_model == NULL) return;

    frame_idx = (frame_idx + 1) % FB_COUNT;

    t3d_mat4fp_from_srt_euler(&model_mats[frame_idx],
        (float[3]){MODEL_SCALE, MODEL_SCALE, MODEL_SCALE},
        (float[3]){model_pitch, model_yaw, model_roll},
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
    return 0;
}
