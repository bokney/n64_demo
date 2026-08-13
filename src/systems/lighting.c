#include "lighting.h"
#include "lighting_components.h"
#include "../ecs_components.h"
#include <t3d/t3d.h>

void lighting_system_apply(void) {
    entity_t active = MAX_ENTITIES;
    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (entity_alive[e] && has_lighting[e] && lightings[e].is_active) {
            active = e;
            break;
        }
    }
    if (active == MAX_ENTITIES) return;

    Lighting *l = &lightings[active];
    t3d_light_set_ambient(l->ambient);
    t3d_light_set_directional(0, l->direction_color, &l->direction);
    t3d_light_set_count(1);
}
