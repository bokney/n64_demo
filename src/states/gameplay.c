#include "gameplay.h"
#include "../game.h"
#include "../ecs.h"
#include "../systems/input.h"
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

#define FB_COUNT 3

#define SCROLL_SPEED 10.0f
#define CAM_HEIGHT 100.0f
#define CAM_FOV 45.0f
#define PLAYER_SPEED 3.0f

static T3DModel *terrain_model = NULL;
static T3DMat4FP *terrain_mats = NULL;
static entity_t scroll_cam_entity = MAX_ENTITIES;
static entity_t player_entity = MAX_ENTITIES;
static int frame_idx = 0;

void gameplay_init(void) {
    terrain_model = t3d_model_load("rom:/models/colourful_cube.t3dm");

    scroll_cam_entity = ecs_create_entity();
    ecs_add_position(scroll_cam_entity, (Position){0.0f, CAM_HEIGHT, 0.0f});
    ecs_add_camera(scroll_cam_entity, (Camera){
        .forward = {{0.0f, -0.5f, -0.866f}},
        .up = {{0.0f, 0.866f, -0.5f}},
        .fov = T3D_DEG_TO_RAD(40.0f),
        .near = 1.0f,
        .far = 500.0f,
        .is_ortho = false,
        .is_active = true
    });
    ecs_add_camera_behavior(scroll_cam_entity, (CameraBehavior){
        .type = CAMERA_BEHAVIOR_SCROLL,
        .scroll = { .scroll_speed_x = 0.0f, .scroll_speed_z = SCROLL_SPEED, .current_offset_x = 0.0f, .current_offset_z = 0.0f }
    });

    // Player entity (2D sprite)
    player_entity = ecs_create_entity();
    ecs_add_position(player_entity, (Position){160.0f, 180.0f, 0.0f});
    ecs_add_sprite(player_entity, (Sprite){24, 24, RGBA32(0x00, 0xff, 0x88, 0xff)});
    ecs_add_input_mover(player_entity, (InputMover){
        .speed = PLAYER_SPEED,
        .move_up = ACTION_UP,
        .move_down = ACTION_DOWN,
        .move_left = ACTION_LEFT,
        .move_right = ACTION_RIGHT
    });

    terrain_mats = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    frame_idx = 0;
}

uint8_t gameplay_update(void) {
    if (input_action_pressed(ACTION_CANCEL)) {
        return STATE_TITLE_SCREEN;
    }

    return 0;
}

void gameplay_render_3d(T3DViewport *viewport) {
    if (terrain_model == NULL) return;

    frame_idx = (frame_idx + 1) % FB_COUNT;

    CameraBehavior *cb = ecs_get_camera_behavior(scroll_cam_entity);
    float scroll_offset_x = cb ? cb->scroll.current_offset_x : 0.0f;
    float scroll_offset_z = cb ? cb->scroll.current_offset_z : 0.0f;

    t3d_mat4fp_from_srt_euler(&terrain_mats[frame_idx],
        (float[3]){100.0f, 1.0f, 1000.0f},
        (float[3]){0.0f, 0.0f, 0.0f},
        (float[3]){-scroll_offset_x, -0.5f, -scroll_offset_z}
    );

    t3d_frame_start();
    t3d_viewport_attach(viewport);

    t3d_screen_clear_color(RGBA32(0x1a, 0x1a, 0x2e, 0xff));
    t3d_screen_clear_depth();

    static const uint8_t color_ambient[4] = {60, 60, 80, 0xFF};
    static const uint8_t color_dir[4] = {0xCC, 0xAA, 0x88, 0xFF};
    t3d_light_set_ambient(color_ambient);
    fm_vec3_t ldir = {{-0.5f, -1.0f, -0.5f}};
    fm_vec3_norm(&ldir, &ldir);
    t3d_light_set_directional(0, color_dir, &ldir);
    t3d_light_set_count(1);

    t3d_matrix_push(&terrain_mats[frame_idx]);
    t3d_model_draw(terrain_model);
    t3d_matrix_pop(1);
}

uint8_t gameplay_exit(void) {
    if (terrain_model) {
        t3d_model_free(terrain_model);
        terrain_model = NULL;
    }
    if (terrain_mats) {
        free_uncached(terrain_mats);
        terrain_mats = NULL;
    }
    if (scroll_cam_entity != MAX_ENTITIES) {
        ecs_destroy_entity(scroll_cam_entity);
        scroll_cam_entity = MAX_ENTITIES;
    }
    if (player_entity != MAX_ENTITIES) {
        ecs_destroy_entity(player_entity);
        player_entity = MAX_ENTITIES;
    }
    return 0;
}
