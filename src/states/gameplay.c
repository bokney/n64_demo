#include "gameplay.h"
#include "../systems/input.h"
#include "../game.h"
#include "../ecs.h"
#include "../systems/transform_components.h"
#include "../systems/render_components.h"
#include "../systems/camera_components.h"
#include "../systems/lighting_components.h"
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

static entity_t cam_entity = MAX_ENTITIES;
static entity_t boat_entity = MAX_ENTITIES;
static entity_t lighting_entity = MAX_ENTITIES;

static float prev_x = 0.0f;
static int travel_dir = 1;
#define BOUND_X 40.0f
#define SPEED_X 0.2f

void gameplay_init(void) {
    prev_x = 10.0f;
    travel_dir = 1;

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

    boat_entity = ecs_create_entity();
    ecs_add_position(boat_entity, (Position){10.0f, 0.0f, 0.0f});
    ecs_add_scale(boat_entity, (Scale){0.1f, 0.1f, 0.1f});
    ecs_add_rotation(boat_entity, (Rotation){0.0f, 0.0f, 0.0f});
    ecs_add_mesh(boat_entity, (Mesh){.model = t3d_model_load("rom:/models/boat-speed-c.t3dm")});

    lighting_entity = ecs_create_entity();
    fm_vec3_t ldir = {{-1.0f, 1.0f, 1.0f}};
    fm_vec3_norm(&ldir, &ldir);
    ecs_add_lighting(lighting_entity, (Lighting){
        .ambient = {80, 80, 100, 0xFF},
        .direction_color = {0xEE, 0xAA, 0xAA, 0xFF},
        .direction = ldir,
        .is_active = true
    });
}

uint8_t gameplay_update(void) {
    Position *pos = ecs_get_position(boat_entity);
    Rotation *rot = ecs_get_rotation(boat_entity);
    if (pos && rot) {
        prev_x = pos->x;
        pos->x += travel_dir * SPEED_X;
        if (pos->x > BOUND_X) {
            pos->x = BOUND_X;
            travel_dir = -1;
        } else if (pos->x < -BOUND_X) {
            pos->x = -BOUND_X;
            travel_dir = 1;
        }
        float dx = pos->x - prev_x;
        rot->yaw = (dx > 0.0f) ? -1.57079633f : (dx < 0.0f) ? 1.57079633f : rot->yaw;
    }

    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_GAME_OVER;
    }
    return 0;
}

uint8_t gameplay_exit(void) {
    Mesh *mesh = ecs_get_mesh(boat_entity);
    if (mesh && mesh->model) {
        t3d_model_free(mesh->model);
    }
    ecs_destroy_entity(&boat_entity);
    ecs_destroy_entity(&lighting_entity);
    ecs_destroy_entity(&cam_entity);
    return 0;
}
