#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <stdint.h>
#include <t3d/t3d.h>

void main_menu_init(void);
uint8_t main_menu_update(void);
uint8_t main_menu_exit(void);
void main_menu_render_3d(T3DViewport *viewport);

#endif