#include "render.h"
#include "../state.h"
#include "camera.h"
#include "t3d/t3d.h"

#define FB_COUNT 3

static T3DViewport viewport;
static bool viewport_initialized = false;

void render_tick(surface_t *disp, state *current) {
    (void)disp;

    if (!viewport_initialized) {
        viewport = t3d_viewport_create_buffered(FB_COUNT);
        viewport_initialized = true;
    }

    camera_system_apply(&viewport);
}
