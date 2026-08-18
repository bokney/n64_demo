#include "main_menu.h"
#include "../ecs.h"
#include "../systems/input.h"
#include "../game.h"
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

#define MODEL_SCALE 0.1f
#define MODEL_ROT_SPEED  0.03f
#define CAM_FOV_SPEED    0.5f
#define CAM_FOV_MIN      20.0f
#define CAM_FOV_MAX      120.0f

static entity_t cam_entity = MAX_ENTITIES;
static entity_t cube_entity = MAX_ENTITIES;
static entity_t lighting_entity = MAX_ENTITIES;

static sprite_t *smile = NULL;
static entity_t smile_entity = MAX_ENTITIES;
static sprite_t *dream = NULL;
static entity_t dream_entity = MAX_ENTITIES;

void main_menu_init(void) {
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

    cube_entity = ecs_create_entity();
    ecs_add_position(cube_entity, (Position){0.0f, 0.0f, 0.0f});
    ecs_add_rotation(cube_entity, (Rotation){0.0f, 0.0f, 0.0f});
    ecs_add_scale(cube_entity, (Scale){MODEL_SCALE, MODEL_SCALE, MODEL_SCALE});
    ecs_add_mesh(cube_entity, (Mesh){.model = t3d_model_load("rom:/models/colourful_cube.t3dm")});

    lighting_entity = ecs_create_entity();
    fm_vec3_t ldir = {{-1.0f, 1.0f, 1.0f}};
    fm_vec3_norm(&ldir, &ldir);
    ecs_add_lighting(lighting_entity, (Lighting){
        .ambient = {80, 80, 100, 0xFF},
        .direction_color = {0xEE, 0xAA, 0xAA, 0xFF},
        .direction = ldir,
        .is_active = true
    });

        smile = sprite_load("rom:/sprites/smile.sprite");
    dream = sprite_load("rom:/sprites/dream.sprite");

    if (smile) {
        smile_entity = ecs_create_entity();
        if (smile_entity < MAX_ENTITIES) {
            ecs_add_sprite(smile_entity, (Sprite){
                .sprite   = smile,
                .x        = 104.0f,
                .y        = 120.0f,
                .scale    = 1.0f,
                .rotation = 0.0f,
                .visible  = true,
            });
        }
    }

    if (dream) {
        dream_entity = ecs_create_entity();
        if (dream_entity < MAX_ENTITIES) {
            ecs_add_sprite(dream_entity, (Sprite){
                .sprite   = dream,
                .x        = 216.0f,
                .y        = 120.0f,
                .scale    = 1.0f,
                .rotation = 0.0f,
                .visible  = true,
            });
        }
    }
}

uint8_t main_menu_update(void) {
    Camera *cam = ecs_get_camera(cam_entity);
    Rotation *rot = ecs_get_rotation(cube_entity);

    if (cam && rot) {
        if (input_action_held(ACTION_C_LEFT))  rot->yaw   += MODEL_ROT_SPEED;
        if (input_action_held(ACTION_C_RIGHT)) rot->yaw   -= MODEL_ROT_SPEED;
        if (input_action_held(ACTION_C_UP))    rot->pitch += MODEL_ROT_SPEED;
        if (input_action_held(ACTION_C_DOWN))  rot->pitch -= MODEL_ROT_SPEED;
        if (input_action_held(ACTION_L)) rot->roll -= MODEL_ROT_SPEED;
        if (input_action_held(ACTION_R)) rot->roll += MODEL_ROT_SPEED;

        if (input_action_held(ACTION_UP)) {
            cam->fov -= CAM_FOV_SPEED * T3D_DEG_TO_RAD(1.0f);
            if (cam->fov < T3D_DEG_TO_RAD(CAM_FOV_MIN)) cam->fov = T3D_DEG_TO_RAD(CAM_FOV_MIN);
        }
        if (input_action_held(ACTION_DOWN)) {
            cam->fov += CAM_FOV_SPEED * T3D_DEG_TO_RAD(1.0f);
            if (cam->fov > T3D_DEG_TO_RAD(CAM_FOV_MAX)) cam->fov = T3D_DEG_TO_RAD(CAM_FOV_MAX);
        }

        if (input_action_pressed(ACTION_CONFIRM)) {
            rot->pitch = rot->yaw = rot->roll = 0.0f;
        }
    }

    if (input_action_pressed(ACTION_PAUSE)) {
        return STATE_GAMEPLAY;
    }

    return 0;
}

uint8_t main_menu_exit(void) {
    if (cube_entity != MAX_ENTITIES) {
        Mesh *mesh = ecs_get_mesh(cube_entity);
        if (mesh && mesh->model) {
            t3d_model_free(mesh->model);
        }
        ecs_destroy_entity(cube_entity);
        cube_entity = MAX_ENTITIES;
    }
    if (lighting_entity != MAX_ENTITIES) {
        ecs_destroy_entity(lighting_entity);
        lighting_entity = MAX_ENTITIES;
    }
    if (cam_entity != MAX_ENTITIES) {
        ecs_destroy_entity(cam_entity);
        cam_entity = MAX_ENTITIES;
    }

    if (smile_entity < MAX_ENTITIES) {
        ecs_destroy_entity(smile_entity);
        smile_entity = MAX_ENTITIES;
    }
    if (smile) {
        sprite_free(smile);
        smile = NULL;
    }
    if (dream_entity < MAX_ENTITIES) {
        ecs_destroy_entity(dream_entity);
        dream_entity = MAX_ENTITIES;
    }
    if (dream) {
        sprite_free(dream);
        dream = NULL;
    }
    return 0;
}
