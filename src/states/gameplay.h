#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stdint.h>
#include <t3d/t3d.h>

void gameplay_init(void);
uint8_t gameplay_update(void);
uint8_t gameplay_exit(void);
void gameplay_render_3d(T3DViewport *viewport);

#endif