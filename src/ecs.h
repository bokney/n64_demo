#ifndef ECS_H
#define ECS_H

#include "ecs_components.h"
#include "systems/camera_components.h"
#include "systems/render_components.h"
#include "systems/lighting_components.h"
#include "systems/input_components.h"

typedef bool (*input_action_held_t)(InputAction action);

entity_t ecs_create_entity(void);
void ecs_destroy_entity(entity_t e);

void ecs_tick_logic(input_action_held_t input_action_held);

#endif
