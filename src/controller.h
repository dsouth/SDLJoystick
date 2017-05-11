//
// Created by Doug South on 10/05/2017.
//

#ifndef JOYSTICK_CONTROLLER_H
#define JOYSTICK_CONTROLLER_H

#include <SDL2/SDL.h>

enum axes {
    LEFT_STICK_X_AXIS = 0,
    LEFT_STICK_Y_AXIS = 1,
    LEFT_TRIGGER_AXIS = 2,
    RIGHT_STICK_X_AXIS = 3,
    RIGHT_STICK_Y_AXIS = 4,
    RIGHT_TRIGGER_AXIS = 5,
};

enum buttons {
    BUTTON_A = 0,
    BUTTON_B = 1,
    BUTTON_X = 2,
    BUTTON_Y = 3,
    LEFT_BUMPER = 4,
    RIGHT_BUMPER = 5,
    LEFT_STICK = 6,
    RIGHT_STICK = 7,
    START_BUTTON = 8,
    BACK_BUTTON = 9,
    X_BOX_BUTTON = 10,
    DPAD_UP = 11,
    DPAD_DOWN = 12,
    DPAD_LEFT = 13,
    DPATH_RIGHT = 14,
};

typedef struct controller_state controller_state;

struct controller_state {
    Sint16 left_x_axis;
    Sint16 left_y_axis;
    Sint16 right_x_axis;
    Sint16 right_y_axis;
    Sint16 left_trigger;
    Sint16 right_trigger;

    Uint8 button_a;
    Uint8 button_b;
    Uint8 button_x;
    Uint8 button_y;
    Uint8 left_bumper;
    Uint8 right_bumper;
    Uint8 left_stick;
    Uint8 right_stick;
    Uint8 button_start;
    Uint8 button_back;
    Uint8 button_x_box;
    Uint8 dpad_up;
    Uint8 dpad_down;
    Uint8 dpad_left;
    Uint8 dpad_right;
};

void init_controller_state(controller_state *s);
int controller_event(SDL_Event e, controller_state* s);

#endif //JOYSTICK_CONTROLLER_H
