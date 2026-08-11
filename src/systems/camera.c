#include "camera.h"
#include "../ecs.h"
#include <t3d/t3d.h>

void camera_system_tick(T3DViewport *viewport) {
    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (!entity_alive[e] || !has_camera_behavior[e] || !has_camera[e]) continue;

        CameraBehavior *cb = &camera_behaviors[e];
        Camera *cam = &cameras[e];
        Position *pos = &positions[e];

        switch (cb->type) {
            case CAMERA_BEHAVIOR_ORBIT: {
                Position *target_pos = ecs_get_position(cb->orbit.target);
                if (!target_pos) break;

                float cp = fm_cosf(cb->orbit.pitch);
                cam->forward.v[0] = cp * fm_sinf(cb->orbit.yaw);
                cam->forward.v[1] = fm_sinf(cb->orbit.pitch);
                cam->forward.v[2] = cp * fm_cosf(cb->orbit.yaw);
                fm_vec3_norm(&cam->forward, &cam->forward);
                cam->up.v[0] = 0; cam->up.v[1] = 1; cam->up.v[2] = 0;

                pos->x = target_pos->x - cam->forward.v[0] * cb->orbit.distance;
                pos->y = target_pos->y - cam->forward.v[1] * cb->orbit.distance;
                pos->z = target_pos->z - cam->forward.v[2] * cb->orbit.distance;
                break;
            }
            case CAMERA_BEHAVIOR_FOLLOW: {
                Position *target_pos = ecs_get_position(cb->follow.target);
                if (!target_pos) break;

                Position desired = {
                    target_pos->x + cb->follow.offset.v[0],
                    target_pos->y + cb->follow.offset.v[1],
                    target_pos->z + cb->follow.offset.v[2]
                };
                float speed = cb->follow.lerp_speed * (1.0f / 60.0f);
                pos->x += (desired.x - pos->x) * speed;
                pos->y += (desired.y - pos->y) * speed;
                pos->z += (desired.z - pos->z) * speed;

                fm_vec3_t to_target;
                fm_vec3_sub(&to_target, (fm_vec3_t *)target_pos, (fm_vec3_t *)pos);
                fm_vec3_norm(&cam->forward, &to_target);
                cam->up.v[0] = 0; cam->up.v[1] = 1; cam->up.v[2] = 0;
                break;
            }
            case CAMERA_BEHAVIOR_SCROLL: {
                cb->scroll.current_offset_x += cb->scroll.scroll_speed_x * (1.0f / 60.0f);
                cb->scroll.current_offset_z += cb->scroll.scroll_speed_z * (1.0f / 60.0f);
                pos->x = -cb->scroll.current_offset_x;
                pos->z = -cb->scroll.current_offset_z;
                break;
            }
            case CAMERA_BEHAVIOR_STATIC:
                break;
            default:
                break;
        }
    }

    entity_t active = MAX_ENTITIES;
    for (entity_t e = 0; e < MAX_ENTITIES; e++) {
        if (entity_alive[e] && has_camera[e] && cameras[e].is_active) {
            active = e;
            break;
        }
    }
    if (active == MAX_ENTITIES) return;

    Camera *cam = &cameras[active];
    Position *pos = &positions[active];

    t3d_viewport_attach(viewport);

    if (cam->is_ortho) {
        t3d_viewport_set_ortho(viewport, cam->left, cam->right, cam->top, cam->bottom, cam->near, cam->far);
    } else {
        t3d_viewport_set_projection(viewport, cam->fov, cam->near, cam->far);
    }

    fm_vec3_t target;
    target.v[0] = pos->x + cam->forward.v[0];
    target.v[1] = pos->y + cam->forward.v[1];
    target.v[2] = pos->z + cam->forward.v[2];
    t3d_viewport_look_at(viewport, (fm_vec3_t *)pos, &target, &cam->up);
}
