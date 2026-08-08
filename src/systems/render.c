#include "render.h"
#include "../ecs.h"
#include <t3d/t3d.h>
#include "../states/main_menu.h"

void render_tick(surface_t *disp, uint32_t state_id) {
    (void)disp;

    if (state_id == STATE_MAIN_MENU) {
        main_menu_render_3d();
        return;
    }

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entity_alive[i]) continue;

        if (has_transform[i] && has_sprite[i]) {
            Sprite *s = &sprites[i];
            Transform *t = &transforms[i];
            rdpq_set_mode_fill(s->color);
            rdpq_fill_rectangle(t->x, t->y, t->x + s->w, t->y + s->h);
        }

        if (has_transform[i] && has_text[i]) {
            Text *txt = &texts[i];
            Transform *t = &transforms[i];
            rdpq_set_mode_standard();
            rdpq_text_printf(NULL, txt->font_id, t->x, t->y, "%s", txt->str);
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

