#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include <stdint.h>
#include <t3d/t3d.h>

void title_screen_init(void);
uint8_t title_screen_update(void);
void title_screen_render(T3DViewport *viewport);
uint8_t title_screen_exit(void);

#endif