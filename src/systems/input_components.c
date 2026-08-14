#include "input_components.h"

InputMover input_movers[MAX_ENTITIES];
bool has_input_mover[MAX_ENTITIES];

void ecs_add_input_mover(entity_t e, InputMover m) {
    if (e >= MAX_ENTITIES) return;
    input_movers[e] = m;
    has_input_mover[e] = true;
}

void ecs_remove_input_mover(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_input_mover[e] = false;
}

bool ecs_has_input_mover(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_input_mover[e];
}

InputMover *ecs_get_input_mover(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &input_movers[e];
}
