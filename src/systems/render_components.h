#ifndef RENDER_COMPONENTS_H
#define RENDER_COMPONENTS_H

#include "../ecs_components.h"
#include <libdragon.h>

typedef struct { uint16_t w, h; color_t color; } Sprite;
typedef struct { const char *str; int font_id; color_t color; } Text;
typedef struct { float v1x, v1y, v2x, v2y, v3x, v3y; color_t color; } Triangle;

extern Sprite sprites[MAX_ENTITIES];
extern bool has_sprite[MAX_ENTITIES];
extern Text texts[MAX_ENTITIES];
extern bool has_text[MAX_ENTITIES];
extern Triangle triangles[MAX_ENTITIES];
extern bool has_triangle[MAX_ENTITIES];

void ecs_add_sprite(entity_t e, Sprite s);
void ecs_remove_sprite(entity_t e);
bool ecs_has_sprite(entity_t e);
Sprite *ecs_get_sprite(entity_t e);

void ecs_add_text(entity_t e, Text t);
void ecs_remove_text(entity_t e);
bool ecs_has_text(entity_t e);
Text *ecs_get_text(entity_t e);

void ecs_add_triangle(entity_t e, Triangle t);
void ecs_remove_triangle(entity_t e);
bool ecs_has_triangle(entity_t e);
Triangle *ecs_get_triangle(entity_t e);

#endif
