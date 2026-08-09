#ifndef ECS_H
#define ECS_H

#include "ecs_components.h"

entity_t ecs_create_entity(void);
void ecs_destroy_entity(entity_t e);

void ecs_tick_logic(void);

#endif