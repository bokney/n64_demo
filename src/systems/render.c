#include "render.h"
#include "../ecs.h"

void render_tick(surface_t *disp) {
    (void)disp;

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
    }
}

