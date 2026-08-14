#include <libdragon.h>
#include <t3d/t3d.h>

#include "game.h"
#include "state.h"
#include "states/state_registry.h"
#include "systems/render.h"
#include "systems/input.h"
#include "systems/camera.h"
#include "ecs.h"

#define LOGIC_RATE_HZ 60
#define TICKS_PER_UPDATE (TICKS_PER_SECOND / LOGIC_RATE_HZ)

int main(void) {
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_DISABLED);
    rdpq_init();
    t3d_init((T3DInitParams){});
    dfs_init(DFS_DEFAULT_LOCATION);
    audio_init(32000, 4);
    mixer_init(32);
    joypad_init();
    timer_init();

    state current_state;
    uint8_t current_state_id = INITIAL_STATE;
    state_registry_load(&current_state, current_state_id);

    uint64_t current_time = timer_ticks();
    uint64_t accumulator = 0;

    for (;;) {
        uint64_t new_time = timer_ticks();
        accumulator += (new_time - current_time);
        current_time = new_time;

        if (audio_can_write()) {
            short *audio_buf = audio_write_begin();
            mixer_poll(audio_buf, audio_get_buffer_length());
            audio_write_end();
        }

        while (accumulator >= TICKS_PER_UPDATE) {
            joypad_poll();
            input_update();
            ecs_tick_logic(input_action_held);
            camera_system_update();

            uint32_t next_state = state_update(&current_state);

            if (next_state != STATE_NONE) {
                if (state_registry_load(&current_state, (uint8_t)next_state) != STATE_NONE) {
                    accumulator = 0;
                    break;
                }
            }

            accumulator -= TICKS_PER_UPDATE;
        }

        surface_t* disp;
        while(!(disp = display_try_get()));
        rdpq_attach_clear(disp, display_get_zbuf());
        render_tick(disp, &current_state);
        rdpq_detach_show();
    }
}
