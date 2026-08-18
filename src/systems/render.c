#include "render.h"
#include "../state.h"
#include "camera.h"
#include "lighting.h"
#include "../ecs.h"
#include <t3d/t3d.h>

#define FB_COUNT 3

static T3DViewport viewport;
static bool viewport_initialized = false;

static T3DMat4FP *model_mats = NULL;
static int frame_idx = 0;

void render_system_draw(T3DViewport *viewport) {
    if (!model_mats) {
        model_mats = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
        frame_idx = 0;
    }

    frame_idx = (frame_idx + 1) % FB_COUNT;

    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (!entity_alive[e] || !has_mesh[e] || !has_position[e]) continue;

        Position *pos = &positions[e];
        float s[3] = {
            has_scale[e] ? scales[e].x : 1.0f,
            has_scale[e] ? scales[e].y : 1.0f,
            has_scale[e] ? scales[e].z : 1.0f
        };
        float r[3] = {
            has_rotation[e] ? rotations[e].pitch : 0.0f,
            has_rotation[e] ? rotations[e].yaw   : 0.0f,
            has_rotation[e] ? rotations[e].roll  : 0.0f
        };
        float t[3] = {pos->x, pos->y, pos->z};

        t3d_mat4fp_from_srt_euler(&model_mats[frame_idx], s, r, t);
        t3d_matrix_push(&model_mats[frame_idx]);
        t3d_model_draw(meshes[e].model);
        t3d_matrix_pop(1);
    }
}

void render_system_draw_2d(void) {
    rdpq_set_mode_standard();
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);

    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (!entity_alive[e]) continue;

        if (has_sprite[e]) {
            Sprite *s = &sprites[e];
            if (s->visible && s->sprite) {
                rdpq_blitparms_t parms = {0};
                parms.cx = s->sprite->width  / 2.0f;
                parms.cy = s->sprite->height / 2.0f;
                parms.scale_x = parms.scale_y = (s->scale > 0.0f) ? s->scale : 1.0f;
                parms.theta = s->rotation;
                rdpq_sprite_blit(s->sprite, s->x, s->y, &parms);
            }
        }

	/*
        if (has_triangle[e]) {
            Triangle *t = &triangles[e];
            float v1[2] = {t->v1x, t->v1y};
            float v2[2] = {t->v2x, t->v2y};
            float v3[2] = {t->v3x, t->v3y};
            rdpq_set_mode_fill(t->color);
            rdpq_triangle(&TRIFMT_FILL, v1, v2, v3);
        }
	*/
    }
}

void render_tick(surface_t *disp, state *current) {
    (void)disp;
    (void)current;

    if (!viewport_initialized) {
        viewport = t3d_viewport_create_buffered(FB_COUNT);
        viewport_initialized = true;
    }

    t3d_frame_start();
    rdpq_mode_dithering(DITHER_NONE_NONE);
    camera_system_apply(&viewport);

    t3d_screen_clear_color(RGBA32(0x00, 0x00, 0x00, 0x00));
    t3d_screen_clear_depth();

    lighting_system_apply();
    render_system_draw(&viewport);

    render_system_draw_2d();
}
