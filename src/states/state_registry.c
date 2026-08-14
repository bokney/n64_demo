#include "state_registry.h"

#include "splash_screen.h"
#include "title_screen.h"
#include "main_menu.h"
#include "gameplay.h"
#include "game_over.h"

uint8_t state_registry_load(state *target, uint8_t state_id) {
    switch (state_id) {
        case STATE_SPLASH:
            assign_state(target, splash_screen_init, splash_screen_update, splash_screen_exit);
            break;
        case STATE_TITLE_SCREEN:
            assign_state(target, title_screen_init, title_screen_update, title_screen_exit);
            break;
        case STATE_MAIN_MENU:
            assign_state(target, main_menu_init, main_menu_update, main_menu_exit);
            break;
        case STATE_GAMEPLAY:
            assign_state(target, gameplay_init, gameplay_update, gameplay_exit);
            break;
        case STATE_GAME_OVER:
            assign_state(target, game_over_init, game_over_update, game_over_exit);
            break;
        default:
            return STATE_NONE;
    }
    return state_id;
}
