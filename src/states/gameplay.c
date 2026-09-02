#include "gameplay.h"
#include "../systems/input.h"
#include "../game.h"

void gameplay_init(void) {
}

uint8_t gameplay_update(void) {
    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_GAME_OVER;
    }
    return 0;
}

uint8_t gameplay_exit(void) {
    return 0;
}
