#include "render.h"
#include "../ecs.h"
#include "../systems/camera.h"
#include <t3d/t3d.h>
#include "../states/main_menu.h"
#include "../states/gameplay.h"

#define FB_COUNT 3

static T3DViewport viewport;
static bool viewport_initialized = false;

void render_tick(surface_t *disp, uint32_t state_id) {
    (void)disp;

    if (!viewport_initialized) {
        viewport = t3d_viewport_create_buffered(FB_COUNT);
        viewport_initialized = true;
    }

    if (state_id == STATE_MAIN_MENU) {
        camera_system_tick(&viewport);
        main_menu_render_3d(&viewport);
        return;
    }

    if (state_id == STATE_GAMEPLAY) {
        camera_system_tick(&viewport);
        gameplay_render_3d(&viewport);

        // 2D foreground pass - use already-attached disp from main.c
        for (int i = 0; i < MAX_ENTITIES; i++) {
            if (!entity_alive[i]) continue;

            if (has_position[i] && has_sprite[i]) {
                Sprite *s = &sprites[i];
                Position *p = &positions[i];
                rdpq_set_mode_fill(s->color);
                rdpq_fill_rectangle((int)p->x, (int)p->y, (int)(p->x + s->w), (int)(p->y + s->h));
            }

            if (has_position[i] && has_text[i]) {
                Text *txt = &texts[i];
                Position *p = &positions[i];
                rdpq_set_mode_standard();
                rdpq_text_printf(NULL, txt->font_id, (int)p->x, (int)p->y, "%s", txt->str);
            }

            if (has_triangle[i]) {
                Triangle *tri = &triangles[i];
                rdpq_set_mode_standard();
                rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
                rdpq_set_prim_color(tri->color);
                float v1[] = {tri->v1x, tri->v1y};
                float v2[] = {tri->v2x, tri->v2y};
                float v3[] = {tri->v3x, tri->v3y};
                rdpq_triangle(&TRIFMT_FILL, v1, v2, v3);
            }
        }
        return;
    }

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entity_alive[i]) continue;

        if (has_position[i] && has_sprite[i]) {
            Sprite *s = &sprites[i];
            Position *p = &positions[i];
            rdpq_set_mode_fill(s->color);
            rdpq_fill_rectangle((int)p->x, (int)p->y, (int)(p->x + s->w), (int)(p->y + s->h));
        }

        if (has_position[i] && has_text[i]) {
            Text *txt = &texts[i];
            Position *p = &positions[i];
            rdpq_set_mode_standard();
            rdpq_text_printf(NULL, txt->font_id, (int)p->x, (int)p->y, "%s", txt->str);
        }

        if (has_triangle[i]) {
            Triangle *tri = &triangles[i];
            rdpq_set_mode_standard();
            rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
            rdpq_set_prim_color(tri->color);
            float v1[] = {tri->v1x, tri->v1y};
            float v2[] = {tri->v2x, tri->v2y};
            float v3[] = {tri->v3x, tri->v3y};
            rdpq_triangle(&TRIFMT_FILL, v1, v2, v3);
        }
    }
}
