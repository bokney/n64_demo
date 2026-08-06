
#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <libdragon.h>

typedef enum {
    ACTION_CONFIRM,
    ACTION_CANCEL,
    ACTION_PAUSE,
    ACTION_UP,
    ACTION_DOWN,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_COUNT
} InputAction;

void input_update(void);

bool input_action_pressed(InputAction action);
bool input_action_held(InputAction action);
bool input_action_released(InputAction action);

#endif
