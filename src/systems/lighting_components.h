#ifndef LIGHTING_COMPONENTS_H
#define LIGHTING_COMPONENTS_H

#include "../ecs_components.h"
#include <libdragon.h>
#include <t3d/t3d.h>

typedef struct {
    uint8_t ambient[4];
    uint8_t direction_color[4];
    fm_vec3_t direction;
    bool is_active;
} Lighting;

extern Lighting lightings[MAX_ENTITIES];
extern bool has_lighting[MAX_ENTITIES];

void ecs_add_lighting(entity_t e, Lighting l);
void ecs_remove_lighting(entity_t e);
bool ecs_has_lighting(entity_t e);
Lighting *ecs_get_lighting(entity_t e);

#endif
