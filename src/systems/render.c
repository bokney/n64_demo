#include "render.h"
#include "../ecs.h"

typedef enum {
    DRAW_FILL_RECT,
    DRAW_TEXT,
} DrawCmdType;

typedef struct {
    DrawCmdType type;
    int16_t x, y;
    int16_t w, h;
    color_t color;
    int font_id;
    const char *str;
} DrawCmd;

#define MAX_DRAW_CMDS 128
static DrawCmd draw_queue[MAX_DRAW_CMDS];
static int draw_queue_count;

static void enqueue_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, color_t color) {
    DrawCmd *cmd = &draw_queue[draw_queue_count++];
    cmd->type = DRAW_FILL_RECT;
    cmd->x = x;
    cmd->y = y;
    cmd->w = w;
    cmd->h = h;
    cmd->color = color;
}

static void enqueue_text(int16_t x, int16_t y, int font_id, color_t color, const char *str) {
    DrawCmd *cmd = &draw_queue[draw_queue_count++];
    cmd->type = DRAW_TEXT;
    cmd->x = x;
    cmd->y = y;
    cmd->font_id = font_id;
    cmd->color = color;
    cmd->str = str;
}

void render_tick(surface_t *disp) {
    (void)disp;
    draw_queue_count = 0;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entity_alive[i]) continue;

        if (has_transform[i] && has_sprite[i]) {
            Sprite *s = &sprites[i];
            Transform *t = &transforms[i];
            enqueue_fill_rect(t->x, t->y, s->w, s->h, s->color);
        }

        if (has_transform[i] && has_text[i]) {
            Text *txt = &texts[i];
            Transform *t = &transforms[i];
            enqueue_text(t->x, t->y, txt->font_id, txt->color, txt->str);
        }
    }

    render_flush(disp);
}

void render_flush(surface_t *disp) {
    (void)disp;
    for (int i = 0; i < draw_queue_count; i++) {
        DrawCmd *cmd = &draw_queue[i];
        switch (cmd->type) {
            case DRAW_FILL_RECT:
                rdpq_set_mode_fill(cmd->color);
                rdpq_fill_rectangle(cmd->x, cmd->y, cmd->x + cmd->w, cmd->y + cmd->h);
                break;
            case DRAW_TEXT:
                rdpq_set_mode_standard();
                rdpq_text_printf(NULL, cmd->font_id, cmd->x, cmd->y, "%s", cmd->str);
                break;
        }
    }
    draw_queue_count = 0;
}