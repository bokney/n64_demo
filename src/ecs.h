#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <stdbool.h>
#include <libdragon.h>
#include "systems/input.h"

#define MAX_ENTITIES 64

typedef uint16_t entity_t;

typedef struct { int16_t x, y; } Transform;
typedef struct { uint16_t w, h; color_t color; } Sprite;
typedef struct { const char *str; int font_id; color_t color; } Text;
typedef struct { int16_t speed; InputAction move_up, move_down, move_left, move_right; } InputMover;

extern Transform transforms[MAX_ENTITIES];
extern bool has_transform[MAX_ENTITIES];
extern Sprite sprites[MAX_ENTITIES];
extern bool has_sprite[MAX_ENTITIES];
extern Text texts[MAX_ENTITIES];
extern bool has_text[MAX_ENTITIES];
extern bool entity_alive[MAX_ENTITIES];

entity_t ecs_create_entity(void);
void ecs_destroy_entity(entity_t e);

void ecs_add_transform(entity_t e, Transform t);
void ecs_add_sprite(entity_t e, Sprite t);
void ecs_add_text(entity_t e, Text t);
void ecs_add_input_mover(entity_t e, InputMover t);

void ecs_remove_transform(entity_t e);
void ecs_remove_sprite(entity_t e);
void ecs_remove_text(entity_t e);
void ecs_remove_input_mover(entity_t e);

bool ecs_has_transform(entity_t e);
bool ecs_has_sprite(entity_t e);
bool ecs_has_text(entity_t e);
bool ecs_has_input_mover(entity_t e);

Transform *ecs_get_transform(entity_t e);
Sprite *ecs_get_sprite(entity_t e);
Text *ecs_get_text(entity_t e);
InputMover *ecs_get_input_mover(entity_t e);

void ecs_tick_logic(void);

#endif