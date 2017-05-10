//
// Created by Doug South on 10/05/2017.
//
#include <SDL2/SDL.h>
#include "controller.h"

void set_button_state(SDL_Event e, Uint8 button_state, controller_state *s) {
    switch (e.jbutton.button) {
        case BUTTON_A:
            s->button_a = button_state;
            break;
        case BUTTON_B:
            s->button_b = button_state;
            break;
        case BUTTON_X:
            s->button_x = button_state;
            break;
        case BUTTON_Y:
            s->button_y = button_state;
            break;
        case LEFT_BUMPER:
            s->left_bumper = button_state;
            break;
        case RIGHT_BUMPER:
            s->right_bumper = button_state;
            break;
        case LEFT_STICK:
            s->left_stick = button_state;
            break;
        case RIGHT_STICK:
            s->right_stick = button_state;
            break;
        case START_BUTTON:
            s->button_start = button_state;
            break;
        case BACK_BUTTON:
            s->button_back = button_state;
            break;
        case X_BOX_BUTTON:
            s->button_x_box = button_state;
            break;
        case DPAD_UP:
            s->dpad_up = button_state;
            break;
        case DPAD_DOWN:
            s->dpad_down = button_state;
            break;
        case DPAD_LEFT:
            s->dpad_left = button_state;
            break;
        case DPATH_RIGHT:
            s->dpad_right = button_state;
            break;
        default:
            break;
    }
}

// This assumes xbox controller
int controller_event(SDL_Event e, controller_state *s) {
    int consumed = 0;
    if (e.type == SDL_JOYAXISMOTION) {
        switch (e.jaxis.axis) {
            case LEFT_STICK_X_AXIS:
                s->left_x_axis = e.jaxis.value;
                break;
            case LEFT_STICK_Y_AXIS:
                s->left_y_axis = e.jaxis.value;
                break;
            case RIGHT_STICK_X_AXIS:
                s->right_x_axis = e.jaxis.value;
                break;
            case RIGHT_STICK_Y_AXIS:
                s->right_y_axis = e.jaxis.value;
                break;
            case LEFT_TRIGGER_AXIS:
                s->left_trigger = e.jaxis.value;
                break;
            case RIGHT_TRIGGER_AXIS:
                s->right_trigger = e.jaxis.value;
                break;
            default:
                break;
        }
        consumed = 1;
    } else if (e.type == SDL_JOYBUTTONDOWN) {
        set_button_state(e, SDL_PRESSED, s);
        consumed = 1;
    } else if (e.type == SDL_JOYBUTTONUP) {
        consumed = 1;
        set_button_state(e, SDL_RELEASED, s);
    }
    return consumed;
}
