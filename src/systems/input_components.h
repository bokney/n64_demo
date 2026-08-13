#ifndef INPUT_COMPONENTS_H
#define INPUT_COMPONENTS_H

#include "../ecs_components.h"
#include "input.h"

typedef struct { int16_t speed; InputAction move_up, move_down, move_left, move_right; } InputMover;

extern InputMover input_movers[MAX_ENTITIES];
extern bool has_input_mover[MAX_ENTITIES];

void ecs_add_input_mover(entity_t e, InputMover m);
void ecs_remove_input_mover(entity_t e);
bool ecs_has_input_mover(entity_t e);
InputMover *ecs_get_input_mover(entity_t e);

#endif
