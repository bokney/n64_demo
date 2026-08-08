#include "main_menu.h"
#include "../ecs.h"
#include "../game.h"
#include "../systems/input.h"
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

#define FB_COUNT 3

static T3DModel *bug_model = NULL;
static T3DViewport viewport;
static T3DMat4FP *model_mats = NULL;
static int frame_idx = 0;
static float elapsed = 0.0f;

static const uint8_t color_ambient[4] = {80, 80, 100, 0xFF};
static const uint8_t color_dir[4]     = {0xEE, 0xAA, 0xAA, 0xFF};

void main_menu_init(void) {
    bug_model = t3d_model_load("rom:/models/colourful_bug.t3dm");

    viewport = t3d_viewport_create_buffered(FB_COUNT);
    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(60.0f), 10.0f, 150.0f);

    const fm_vec3_t cam_pos    = {{0.0f, 12.0f, 25.0f}};
    const fm_vec3_t cam_target = {{0.0f, 0.0f, 0.0f}};
    t3d_viewport_look_at(&viewport, &cam_pos, &cam_target, &(fm_vec3_t){{0,1,0}});

    model_mats = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    elapsed = 0.0f;
    frame_idx = 0;
}

uint8_t main_menu_update(void) {
    elapsed += 1.0f / 60.0f;

    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_SPLASH;
    }
    return 0;
}

void main_menu_render_3d(void) {
    if (bug_model == NULL) return;

    frame_idx = (frame_idx + 1) % FB_COUNT;

    float osc_y = fm_sinf(elapsed * 2.0f) * 3.0f;

    t3d_mat4fp_from_srt_euler(&model_mats[frame_idx],
        (float[3]){1.0f, 1.0f, 1.0f},
        (float[3]){0.0f, 0.0f, 0.0f},
        (float[3]){0.0f, osc_y, 0.0f}
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
    t3d_model_draw(bug_model);
    t3d_matrix_pop(1);
}

uint8_t main_menu_exit(void) {
    if (bug_model) {
        t3d_model_free(bug_model);
        bug_model = NULL;
    }
    if (model_mats) {
        free_uncached(model_mats);
        model_mats = NULL;
    }
    t3d_viewport_destroy(&viewport);
    return 0;
}
