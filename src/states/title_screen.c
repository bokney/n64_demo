#include <libdragon.h>
#include "title_screen.h"
#include "../ecs.h"
#include "../game.h"
#include "../systems/input.h"

#define SCREEN_W 320.0f
#define SCREEN_H 240.0f
#define CENTER_X (SCREEN_W * 0.5f)
#define CENTER_Y (SCREEN_H * 0.5f)
#define LOGO_SIZE 70.0f

#define ROT_SPEED 0.3f

#define FREQ_V1_X 0.7f
#define FREQ_V1_Y 1.1f
#define FREQ_V2_X 0.9f
#define FREQ_V2_Y 0.5f
#define FREQ_V3_X 1.3f
#define FREQ_V3_Y 0.8f

#define AMP_V1_X 35.0f
#define AMP_V1_Y 25.0f
#define AMP_V2_X 30.0f
#define AMP_V2_Y 40.0f
#define AMP_V3_X 25.0f
#define AMP_V3_Y 35.0f

static entity_t tri_entity;
static float elapsed;

static sprite_t *smile = NULL;
static entity_t smile_entity = MAX_ENTITIES;
static sprite_t *dream = NULL;
static entity_t dream_entity = MAX_ENTITIES;

static xm64player_t music;

void title_screen_init(void) {
    elapsed = 0.0f;

    tri_entity = ecs_create_entity();
    Triangle tri = {
        .v1x = CENTER_X,
        .v1y = CENTER_Y - LOGO_SIZE,
        .v2x = CENTER_X - LOGO_SIZE * 0.9f,
        .v2y = CENTER_Y + LOGO_SIZE * 0.8f,
        .v3x = CENTER_X + LOGO_SIZE * 0.9f,
        .v3y = CENTER_Y + LOGO_SIZE * 0.8f,
        .color = RGBA32(0x5b, 0x6e, 0xe1, 0xff)
    };
    ecs_add_triangle(tri_entity, tri);

    smile = sprite_load("rom:/sprites/smile.sprite");
    dream = sprite_load("rom:/sprites/dream.sprite");

    if (smile) {
        smile_entity = ecs_create_entity();
        if (smile_entity < MAX_ENTITIES) {
            ecs_add_sprite(smile_entity, (Sprite){
                .sprite   = smile,
                .x        = 104.0f,
                .y        = 120.0f,
                .scale    = 1.0f,
                .rotation = 0.0f,
                .visible  = true,
            });
        }
    }

    if (dream) {
        dream_entity = ecs_create_entity();
        if (dream_entity < MAX_ENTITIES) {
            ecs_add_sprite(dream_entity, (Sprite){
                .sprite   = dream,
                .x        = 216.0f,
                .y        = 120.0f,
                .scale    = 1.0f,
                .rotation = 0.0f,
                .visible  = true,
            });
        }
    }

    xm64player_open(&music, "rom:/audio/convertable_drive.xm64");
    xm64player_play(&music, 0);
}

uint8_t title_screen_update(void) {
    elapsed += 1.0f / 60.0f;

    float bx1 = 0.0f,        by1 = -LOGO_SIZE;
    float bx2 = -LOGO_SIZE * 0.9f, by2 = LOGO_SIZE * 0.8f;
    float bx3 =  LOGO_SIZE * 0.9f, by3 = LOGO_SIZE * 0.8f;

    float mx1 = bx1 + AMP_V1_X * fm_sinf(elapsed * FREQ_V1_X);
    float my1 = by1 + AMP_V1_Y * fm_sinf(elapsed * FREQ_V1_Y);
    float mx2 = bx2 + AMP_V2_X * fm_sinf(elapsed * FREQ_V2_X);
    float my2 = by2 + AMP_V2_Y * fm_sinf(elapsed * FREQ_V2_Y);
    float mx3 = bx3 + AMP_V3_X * fm_sinf(elapsed * FREQ_V3_X);
    float my3 = by3 + AMP_V3_Y * fm_sinf(elapsed * FREQ_V3_Y);

    float s, c;
    fm_sincosf(elapsed * ROT_SPEED, &s, &c);

    Triangle *tri = ecs_get_triangle(tri_entity);
    tri->v1x = CENTER_X + mx1 * c - my1 * s;
    tri->v1y = CENTER_Y + mx1 * s + my1 * c;
    tri->v2x = CENTER_X + mx2 * c - my2 * s;
    tri->v2y = CENTER_Y + mx2 * s + my2 * c;
    tri->v3x = CENTER_X + mx3 * c - my3 * s;
    tri->v3y = CENTER_Y + mx3 * s + my3 * c;

    if (input_action_pressed(ACTION_CONFIRM)) {
        return STATE_MAIN_MENU;
    }
    return 0;
}

uint8_t title_screen_exit(void) {
    ecs_destroy_entity(tri_entity);

    if (smile_entity < MAX_ENTITIES) {
        ecs_destroy_entity(smile_entity);
        smile_entity = MAX_ENTITIES;
    }
    if (smile) {
        sprite_free(smile);
        smile = NULL;
    }

    if (dream_entity < MAX_ENTITIES) {
        ecs_destroy_entity(dream_entity);
        dream_entity = MAX_ENTITIES;
    }
    if (dream) {
        sprite_free(dream);
        dream = NULL;
    }

    xm64player_stop(&music);
    xm64player_close(&music);
    return 0;
}
