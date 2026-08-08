#include "main_menu.h"
#include "../ecs.h"
#include "../game.h"
#include "../systems/input.h"
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

#define FB_COUNT 3

#define MODEL_SCALE 0.1f

static T3DModel *cube_model = NULL;
static T3DViewport viewport;
static T3DMat4FP *model_mats = NULL;
static int frame_idx = 0;

static const uint8_t color_ambient[4] = {80, 80, 100, 0xFF};
static const uint8_t color_dir[4]     = {0xEE, 0xAA, 0xAA, 0xFF};

#define CAM_TURN_SPEED   0.06f
#define CAM_STRAFE_SPEED 1.5f
#define CAM_VERT_SPEED   1.5f
#define CAM_PITCH_MIN    T3D_DEG_TO_RAD(-85.0f)
#define CAM_PITCH_MAX    T3D_DEG_TO_RAD(85.0f)

static fm_vec3_t cam_pos    = {{0.0f, 12.0f, 25.0f}};
static fm_vec3_t cam_target = {{0.0f, 0.0f, 0.0f}};

void main_menu_init(void) {
    cube_model = t3d_model_load("rom:/models/colourful_cube.t3dm");

    viewport = t3d_viewport_create_buffered(FB_COUNT);
    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(60.0f), 10.0f, 150.0f);

    t3d_viewport_look_at(&viewport, &cam_pos, &cam_target, &(fm_vec3_t){{0,1,0}});

    model_mats = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    frame_idx = 0;
}

uint8_t main_menu_update(void) {
    bool turning = input_action_held(ACTION_C_LEFT)  || input_action_held(ACTION_C_RIGHT) ||
                   input_action_held(ACTION_C_UP)    || input_action_held(ACTION_C_DOWN);
    if (turning) {
        fm_vec3_t offset;
        fm_vec3_sub(&offset, &cam_pos, &cam_target);
        float dist = fm_vec3_len(&offset);
        if (dist > 0.001f) {
            float yaw   = atan2f(offset.v[0], offset.v[2]);
            float pitch = asinf(offset.v[1] / dist);

            if (input_action_held(ACTION_C_LEFT))  yaw   -= CAM_TURN_SPEED;
            if (input_action_held(ACTION_C_RIGHT)) yaw   += CAM_TURN_SPEED;
            if (input_action_held(ACTION_C_UP))    pitch += CAM_TURN_SPEED;
            if (input_action_held(ACTION_C_DOWN))  pitch -= CAM_TURN_SPEED;

            if (pitch > CAM_PITCH_MAX) pitch = CAM_PITCH_MAX;
            if (pitch < CAM_PITCH_MIN) pitch = CAM_PITCH_MIN;

            float cp = fm_cosf(pitch);
            cam_pos.v[0] = cam_target.v[0] + dist * cp * fm_sinf(yaw);
            cam_pos.v[1] = cam_target.v[1] + dist * fm_sinf(pitch);
            cam_pos.v[2] = cam_target.v[2] + dist * cp * fm_cosf(yaw);
        }
    }

    if (input_action_held(ACTION_CONFIRM)) {
        fm_vec3_t look;
        fm_vec3_sub(&look, &cam_target, &cam_pos);
        float horiz_len = sqrtf(look.v[0] * look.v[0] + look.v[2] * look.v[2]);
        if (horiz_len > 0.001f) {
            if (input_action_held(ACTION_LEFT)) {
                cam_pos.v[0] -= look.v[2] / horiz_len * CAM_STRAFE_SPEED;
                cam_pos.v[2] += look.v[0] / horiz_len * CAM_STRAFE_SPEED;
            }
            if (input_action_held(ACTION_RIGHT)) {
                cam_pos.v[0] += look.v[2] / horiz_len * CAM_STRAFE_SPEED;
                cam_pos.v[2] -= look.v[0] / horiz_len * CAM_STRAFE_SPEED;
            }
        }
    }

    if (input_action_held(ACTION_CANCEL)) {
        if (input_action_held(ACTION_UP)) {
            cam_pos.v[1] += CAM_VERT_SPEED;
        }
        if (input_action_held(ACTION_DOWN)) {
            cam_pos.v[1] -= CAM_VERT_SPEED;
        }
    }

    t3d_viewport_look_at(&viewport, &cam_pos, &cam_target, &(fm_vec3_t){{0,1,0}});

    bool any_dir = input_action_held(ACTION_LEFT)  || input_action_held(ACTION_RIGHT) ||
                   input_action_held(ACTION_UP)    || input_action_held(ACTION_DOWN);
    /*
    if (input_action_pressed(ACTION_CONFIRM) && !any_dir) {
        return STATE_SPLASH;
    }
    */
    return 0;
}

void main_menu_render_3d(void) {
    if (cube_model == NULL) return;

    frame_idx = (frame_idx + 1) % FB_COUNT;

    t3d_mat4fp_from_srt_euler(&model_mats[frame_idx],
        (float[3]){MODEL_SCALE, MODEL_SCALE, MODEL_SCALE},
        (float[3]){0.0f, 0.0f, 0.0f},
        (float[3]){0.0f, 0.0f, 0.0f}
    );

    t3d_frame_start();
    t3d_viewport_attach(&viewport);

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
    t3d_viewport_destroy(&viewport);
    return 0;
}
