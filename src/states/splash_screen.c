
#include <stdio.h>

#include "splash_screen.h"
#include "../systems/input.h"

static xm64player_t music;
static rdpq_font_t *custom_font;

static int box_x = 100;
static int box_y = 140;

void splash_screen_init(void) {
    xm64player_open(&music, "rom:/audio/boomtiss.xm64");
    xm64player_play(&music, 0);

    custom_font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO); 
    rdpq_text_register_font(1, custom_font);
    
    box_x = 100;
    box_y = 140;
}

uint8_t splash_screen_update(void) {
    int speed = 2;

    if (input_action_held(ACTION_UP)) {
        box_y -= speed;
    }
    if (input_action_held(ACTION_DOWN)) {
        box_y += speed;
    }
    if (input_action_held(ACTION_LEFT)) {
        box_x -= speed;
    }
    if (input_action_held(ACTION_RIGHT)) {
        box_x += speed;
    }

    if (input_action_pressed(ACTION_CONFIRM)) {
        return 2;
    }

    return 0;
}

void splash_screen_draw(surface_t *disp) {
    rdpq_set_mode_fill(RGBA32(195, 167, 225, 0));
    rdpq_fill_rectangle(0, 0, display_get_width(), display_get_height());

    rdpq_set_mode_standard();

    rdpq_text_printf(NULL, 1, 100, 100, "SPLASH SCREEN ACTIVE");
    rdpq_text_printf(NULL, 1, 100, 120, "Hello World!");

    rdpq_set_mode_fill(RGBA32(149, 105, 200, 0)); 
    
    rdpq_fill_rectangle(box_x, box_y, box_x + 50, box_y + 50);
}

uint8_t splash_screen_exit(void) {
    xm64player_stop(&music);
    xm64player_close(&music);

    rdpq_font_free(custom_font);

    return 0;
}
