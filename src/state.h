#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <libdragon.h>
#include <t3d/t3d.h>

typedef struct _state {
    uint8_t phase;
    uint32_t step;
    void (*init)(void);
    uint8_t (*update)(void);
    uint8_t (*exit)(void);
} state;

void assign_state(
    state *target,
    void (*init)(void),
    uint8_t (*update)(void),
    uint8_t (*exit)(void)
);

uint8_t state_update(state *target);

#endif
