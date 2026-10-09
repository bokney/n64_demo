#include "game_over.h"
#include "../systems/input.h"
#include "../game.h"

void game_over_init(void) {
}

uint8_t game_over_update(void) {
    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_SPLASH;
    }
    return 0;
}

uint8_t game_over_exit(void) {
    return 0;
}
