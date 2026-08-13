#include "splash_screen.h"
#include <libdragon.h>
#include "../game.h"
#include "../ecs.h"
#include "../systems/input.h"

static xm64player_t music;
static sprite_t *logo = NULL;
static entity_t logo_entity = MAX_ENTITIES;

void splash_screen_init(void) {
    logo = sprite_load("rom:/sprites/libdragon_logo.sprite");

    if (logo) {
        logo_entity = ecs_create_entity();
        if (logo_entity < MAX_ENTITIES) {
            ecs_add_sprite(logo_entity, (Sprite){
                .sprite   = logo,
                .x        = 160.0f,
                .y        = 120.0f,
                .scale    = 1.0f,
                .rotation = 0.0f,
                .visible  = true,
            });
        }
    }

    xm64player_open(&music, "rom:/audio/boomtiss.xm64");
    xm64player_play(&music, 0);
}

uint8_t splash_screen_update(void) {
    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_TITLE_SCREEN;
    }
    return 0;
}

uint8_t splash_screen_exit(void) {
    if (logo_entity < MAX_ENTITIES) {
        ecs_destroy_entity(logo_entity);
        logo_entity = MAX_ENTITIES;
    }
    if (logo) {
        sprite_free(logo);
        logo = NULL;
    }

    xm64player_stop(&music);
    xm64player_close(&music);
    return 0;
}
