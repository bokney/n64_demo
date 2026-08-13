#include "state.h"

void assign_state(
    state *target,
    void (*init)(void),
    uint8_t (*update)(void),
    void (*render)(T3DViewport *viewport),
    uint8_t (*exit)(void)
) {
    target->phase = 0;
    target->step = 0;
    target->init = init;
    target->update = update;
    target->render = render;
    target->exit = exit;
}

uint8_t state_update(state *target) {
    if (target->phase == 0) {
        if (target->init) target->init();
        target->phase = 1;
    }

    if (target->phase == 1) {
        uint32_t next_state = 0;

        if (target->update) {
            next_state = target->update();
        }
        target->step++;

        if (next_state != 0) {
            target->phase = 2;

            if (target->exit) target->exit();

            target->phase = 0;

            return next_state;
        }
    }

    return 0;
}

void state_draw(state *target, T3DViewport *viewport) {
    if (target->render) {
        target->render(viewport);
    }
}
