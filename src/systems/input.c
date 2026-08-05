
#include "input.h"

#define STICK_DEADZONE 40

static uint32_t current_state = 0;
static uint32_t previous_state = 0;

void input_update(void) {
    previous_state = current_state;
    current_state = 0;

    joypad_inputs_t port1 = joypad_get_inputs(JOYPAD_PORT_1);

    if (port1.btn.a || port1.btn.start) current_state |= (1 << ACTION_CONFIRM);
    if (port1.btn.b)                    current_state |= (1 << ACTION_CANCEL);
    if (port1.btn.start)                current_state |= (1 << ACTION_PAUSE);

    if (port1.btn.d_up    || port1.stick_y > STICK_DEADZONE)  current_state |= (1 << ACTION_UP);
    if (port1.btn.d_down  || port1.stick_y < -STICK_DEADZONE) current_state |= (1 << ACTION_DOWN);
    if (port1.btn.d_left  || port1.stick_x < -STICK_DEADZONE) current_state |= (1 << ACTION_LEFT);
    if (port1.btn.d_right || port1.stick_x > STICK_DEADZONE)  current_state |= (1 << ACTION_RIGHT);
}

bool input_action_pressed(InputAction action) {
    return (current_state & (1 << action)) && !(previous_state & (1 << action));
}

bool input_action_held(InputAction action) {
    return (current_state & (1 << action));
}

bool input_action_released(InputAction action) {
    return !(current_state & (1 << action)) && (previous_state & (1 << action));
}
