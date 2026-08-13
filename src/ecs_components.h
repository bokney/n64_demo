#ifndef ECS_COMPONENTS_H
#define ECS_COMPONENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <libdragon.h>
#include "systems/input.h"

#define MAX_ENTITIES 64

typedef uint16_t entity_t;

typedef struct { float x, y, z; } Position;
typedef struct { float pitch, yaw, roll; } Rotation;
typedef struct { float x, y, z; } Scale;

extern Position positions[MAX_ENTITIES];
extern bool has_position[MAX_ENTITIES];
extern Rotation rotations[MAX_ENTITIES];
extern bool has_rotation[MAX_ENTITIES];
extern Scale scales[MAX_ENTITIES];
extern bool has_scale[MAX_ENTITIES];
extern bool entity_alive[MAX_ENTITIES];

void ecs_add_position(entity_t e, Position p);
void ecs_remove_position(entity_t e);
bool ecs_has_position(entity_t e);
Position *ecs_get_position(entity_t e);

void ecs_add_rotation(entity_t e, Rotation r);
void ecs_remove_rotation(entity_t e);
bool ecs_has_rotation(entity_t e);
Rotation *ecs_get_rotation(entity_t e);

void ecs_add_scale(entity_t e, Scale s);
void ecs_remove_scale(entity_t e);
bool ecs_has_scale(entity_t e);
Scale *ecs_get_scale(entity_t e);

#endif
