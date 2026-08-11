#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <stdint.h>
#include <t3d/t3d.h>

void game_over_init(void);
uint8_t game_over_update(void);
uint8_t game_over_exit(void);
void game_over_render_3d(T3DViewport *viewport);

#endif
