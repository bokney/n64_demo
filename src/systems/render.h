#ifndef RENDER_H
#define RENDER_H

#include <libdragon.h>
#include <t3d/t3d.h>
#include "../state.h"

void render_tick(surface_t *disp, state *current);
void render_system_draw(T3DViewport *viewport);
void render_system_draw_2d(void);

#endif
