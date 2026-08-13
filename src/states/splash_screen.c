#include "splash_screen.h"
#include "../game.h"
#include "../systems/input.h"

static xm64player_t music;

void splash_screen_init(void) {
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
    xm64player_stop(&music);
    xm64player_close(&music);
    return 0;
}
