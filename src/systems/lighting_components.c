#include "lighting_components.h"

Lighting lightings[MAX_ENTITIES];
bool has_lighting[MAX_ENTITIES];

void ecs_add_lighting(entity_t e, Lighting l) {
    if (e >= MAX_ENTITIES) return;
    lightings[e] = l;
    has_lighting[e] = true;
}

void ecs_remove_lighting(entity_t e) {
    if (e >= MAX_ENTITIES) return;
    has_lighting[e] = false;
}

bool ecs_has_lighting(entity_t e) {
    if (e >= MAX_ENTITIES) return false;
    return has_lighting[e];
}

Lighting *ecs_get_lighting(entity_t e) {
    if (e >= MAX_ENTITIES) return NULL;
    return &lightings[e];
}
