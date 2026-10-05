#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <libdragon.h>
#include "systems/input.h"

#define MAX_ENTITIES 64

typedef uint16_t entity_t;

extern bool entity_alive[MAX_ENTITIES];

#endif
