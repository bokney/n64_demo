#include "splash_screen.h"
#include "../ecs.h"
#include "../game.h"
#include "../systems/input.h"
#include "sprite.h"

sprite_t *logo = NULL;

void splash_screen_init(void) {
    logo = sprite_load("rom:/sprites/libdragon_logo.sprite");
}

uint8_t splash_screen_update(void) {
    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_TITLE_SCREEN;
    }
    return 0;
}

uint8_t splash_screen_exit(void) {
    sprite_free(logo);
    logo = NULL;
    return 0;
}
