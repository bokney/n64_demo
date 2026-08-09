#include "splash_screen.h"
#include "../ecs.h"
#include "../game.h"
#include "../systems/input.h"

static xm64player_t music;
static rdpq_font_t *custom_font;
static entity_t box_entity;
static entity_t text_entity;

void splash_screen_init(void) {
    xm64player_open(&music, "rom:/audio/boomtiss.xm64");
    xm64player_play(&music, 0);

    custom_font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
    rdpq_text_register_font(1, custom_font);

    entity_t bg_entity = ecs_create_entity();
    Position bg_p = {0.0f, 0.0f, 0.0f};
    Sprite bg_s = {320, 240, RGBA32(195, 167, 225, 0)};
    ecs_add_position(bg_entity, bg_p);
    ecs_add_sprite(bg_entity, bg_s);

    box_entity = ecs_create_entity();
    Position box_p = {100.0f, 140.0f, 0.0f};
    Sprite box_s = {50, 50, RGBA32(149, 105, 200, 0)};
    InputMover box_m = {2, ACTION_UP, ACTION_DOWN, ACTION_LEFT, ACTION_RIGHT};
    ecs_add_position(box_entity, box_p);
    ecs_add_sprite(box_entity, box_s);
    ecs_add_input_mover(box_entity, box_m);

    text_entity = ecs_create_entity();
    Position text_p = {100.0f, 100.0f, 0.0f};
    Text text_d = {"SPLASH SCREEN ACTIVE", 1, RGBA32(255, 255, 255, 255)};
    ecs_add_position(text_entity, text_p);
    ecs_add_text(text_entity, text_d);
}

uint8_t splash_screen_update(void) {
    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_TITLE_SCREEN;
    }
    return 0;
}

uint8_t splash_screen_exit(void) {
    xm64player_stop(&music);
    xm64player_close(&music);
    rdpq_font_free(custom_font);
    ecs_destroy_entity(box_entity);
    ecs_destroy_entity(text_entity);
    return 0;
}