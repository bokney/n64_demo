#include <stdio.h>
#include <libdragon.h>
#include <t3d/t3d.h>

#include "game.h"
#include "state.h"
#include "states/splash_screen.h"
#include "states/title_screen.h"
#include "states/main_menu.h"
#include "states/gameplay.h"
#include "states/game_over.h"
#include "ecs.h"
#include "systems/render.h"
#include "systems/input.h"

#define LOGIC_RATE_HZ 60
#define TICKS_PER_UPDATE (TICKS_PER_SECOND / LOGIC_RATE_HZ)

#define INITIAL_STATE STATE_SPLASH

void load_state(state *target, uint32_t state_id) {
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
            break;
    }
}

int main(void) {
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    rdpq_init();
    t3d_init((T3DInitParams){});
    dfs_init(DFS_DEFAULT_LOCATION);
    audio_init(32000, 4);
    mixer_init(32);
    joypad_init();
    timer_init();

    state current_state;
    uint32_t current_state_id = INITIAL_STATE;
    load_state(&current_state, INITIAL_STATE);

    uint64_t current_time = timer_ticks();
    uint64_t accumulator = 0;

    for (;;) {
        if (audio_can_write()) {
            short *audio_buf = audio_write_begin();
            mixer_poll(audio_buf, audio_get_buffer_length());
            audio_write_end();
        }

        uint64_t new_time = timer_ticks();
        accumulator += (new_time - current_time);
        current_time = new_time;

        while (accumulator >= TICKS_PER_UPDATE) {
            joypad_poll();
            input_update();
            ecs_tick_logic();

            uint32_t next_state = state_update(&current_state);

            if (next_state != STATE_NONE) {
                current_state_id = next_state;
                load_state(&current_state, next_state);
                accumulator = 0;
                current_time = timer_ticks();
                break;
            }

            accumulator -= TICKS_PER_UPDATE;
        }

        surface_t* disp;
        while(!(disp = display_try_get()));
        rdpq_attach_clear(disp, display_get_zbuf());
        render_tick(disp, current_state_id);
        rdpq_detach_show();
    }
}
