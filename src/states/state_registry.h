#ifndef STATE_REGISTRY_H
#define STATE_REGISTRY_H

#include "../state.h"
#include "../game.h"

#define INITIAL_STATE STATE_SPLASH

uint8_t state_registry_load(state *target, uint8_t state_id);

#endif
