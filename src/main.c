#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "controller.h"

typedef struct texture texture;

struct texture {
    SDL_Texture *t;
    int w;
    int h;
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Renderer *gRenderer;
SDL_Window *gWindow;
SDL_Joystick *gGameController;
texture *button_a;
texture *button_a_pressed;
texture *button_b;
texture *button_b_pressed;
texture *button_x;
texture *button_x_pressed;
texture *button_y;
texture *button_y_pressed;
texture *dpad_up;
texture *dpad_up_pressed;
texture *dpad_down;
texture *dpad_down_pressed;
texture *dpad_left;
texture *dpad_left_pressed;
texture *dpad_right;
texture *dpad_right_pressed;

int init() {
    int success;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = 0;
    } else {
        gWindow = SDL_CreateWindow("SDL Joystick",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = 0;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = 0;
            } else {
                success = 1;

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = 0;
                }

                if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
                    printf("Warning: Linear texture filtering not enabled!\n");
                }
                if (SDL_NumJoysticks() < 1) {
                    printf("Warning: No joysticks connected!\n");
                    success = 0;
                } else {
                    gGameController = SDL_JoystickOpen(0);
                    if (gGameController == NULL) {
                        printf("Warning: unable to open game controller! SDL Error%s\n", SDL_GetError());
                        success = 0;
                    } else {
                        printf("Joystick found with %d axes ", SDL_JoystickNumAxes(gGameController));
                        printf("and %d buttons\n", SDL_JoystickNumButtons(gGameController));
                        SDL_SetWindowTitle(gWindow, SDL_JoystickNameForIndex(0));
                    }
                }
            }
        }
    }
    return success;
}

texture *load_texture(const char *f) {
    texture *t = NULL;
    SDL_Surface *s = IMG_Load(f);
    if (s == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", f, IMG_GetError());
    } else {
        t = (texture *) malloc(sizeof(texture));
        t->t = SDL_CreateTextureFromSurface(gRenderer, s);
        if (t->t == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", f, SDL_GetError());
        }
        t->w = s->w;
        t->h = s->h;
        SDL_FreeSurface(s);
    }
    return t;
}

int loadMedia() {
    button_a = load_texture("../res/button_a.png");
    button_a_pressed = load_texture("../res/button_a_pressed.png");
    button_b = load_texture("../res/button_b.png");
    button_b_pressed = load_texture("../res/button_b_pressed.png");
    button_x = load_texture("../res/button_x.png");
    button_x_pressed = load_texture("../res/button_x_pressed.png");
    button_y = load_texture("../res/button_y.png");
    button_y_pressed = load_texture("../res/button_y_pressed.png");
    dpad_up = load_texture("../res/dpad_up.png");
    dpad_up_pressed = load_texture("../res/dpad_up_pressed.png");
    dpad_down = load_texture("../res/dpad_down.png");
    dpad_down_pressed = load_texture("../res/dpad_down_pressed.png");
    dpad_left = load_texture("../res/dpad_left.png");
    dpad_left_pressed = load_texture("../res/dpad_left_pressed.png");
    dpad_right = load_texture("../res/dpad_right.png");
    dpad_right_pressed = load_texture("../res/dpad_right_pressed.png");
    return button_a != NULL && button_a_pressed != NULL
           && button_b != NULL && button_b_pressed != NULL
           && button_x != NULL && button_x_pressed != NULL
           && button_y != NULL && button_y_pressed != NULL;
}

void set_color_white() {
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
}

void set_color_black() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
}

void render_stick(int bx,
                  int by,
                  int x,
                  int y,
                  Uint8 button) {
    x = x / 512;
    y = y / 512;
    SDL_Rect r = {bx, by, 140, 140};
    if (button == SDL_PRESSED) {
        set_color_black();
        SDL_RenderFillRect(gRenderer, &r);
        set_color_white();
    } else {
        set_color_black();
    }
    SDL_RenderDrawRect(gRenderer, &r);
    if (button == SDL_PRESSED) {
        set_color_white();
    } else {
        set_color_black();
    }
    for (int i = by + 66; i < by + 73; i++) {
        SDL_RenderDrawLine(gRenderer, x + bx + 66,
                           i + y, x + bx + 73, i + y);
    }
}

void render_trigger(int bx, int by, int x) {
    set_color_black();
    x = (x / 512) + 64;
    SDL_Rect r = {bx, by, 18, 129};
    SDL_RenderDrawRect(gRenderer, &r);
    SDL_Rect bar = {bx + 1, 128 - x + by, 16, x};
    SDL_RenderFillRect(gRenderer, &bar);
}

void render_button(texture *on,
                   texture *off,
                   Uint8 state,
                   int x,
                   int y) {
    texture *t = state == SDL_PRESSED ? on : off;
    SDL_Rect r = {x, y, t->w, t->h};
    SDL_RenderCopy(gRenderer, t->t, NULL, &r);
}

void render_abxy(controller_state s, int x, int y) {
    render_button(button_a_pressed, button_a, s.button_a, x + 75, y + 85);
    render_button(button_b_pressed, button_b, s.button_b, x + 150, y + 47);
    render_button(button_x_pressed, button_x, s.button_x, x, y + 47);
    render_button(button_y_pressed, button_y, s.button_y, x + 75, y);

}

void render_dpad(controller_state s, int x, int y) {
    render_button(dpad_up_pressed, dpad_up, s.dpad_up, x + 45, y);
    render_button(dpad_down_pressed, dpad_down, s.dpad_down, x + 45, y + 75);
    render_button(dpad_left_pressed, dpad_left, s.dpad_left, x, y + 45);
    render_button(dpad_right_pressed, dpad_right, s.dpad_right, x + 75, y + 45);
}

void loop() {

    controller_state s;

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else {
                controller_event(e, &s);
            }
        }

        set_color_white();
        SDL_RenderClear(gRenderer);

        render_stick(5, 5, s.left_x_axis, s.left_y_axis, s.left_stick);
        render_stick(150, 5, s.right_x_axis, s.right_y_axis, s.right_stick);
        render_trigger(295, 5, s.left_trigger);
        render_trigger(315, 5, s.right_trigger);

        render_abxy(s, 405, 5);
        render_dpad(s, 55, 175);

        SDL_RenderPresent(gRenderer);
        // update texture here
    }

}

void free_texture(texture *t) {
    if (t != NULL) {
        SDL_DestroyTexture(t->t);
        free(t);
    }
}

void free_resources() {
    free_texture(button_a);
    free_texture(button_a_pressed);
    free_texture(button_b);
    free_texture(button_b_pressed);
    free_texture(button_x);
    free_texture(button_x_pressed);
    free_texture(button_y);
    free_texture(button_y_pressed);
    free_texture(dpad_up);
    free_texture(dpad_up_pressed);
    free_texture(dpad_down);
    free_texture(dpad_down_pressed);
    free_texture(dpad_left);
    free_texture(dpad_left_pressed);
    free_texture(dpad_right);
    free_texture(dpad_right_pressed);
    if (gRenderer != NULL) SDL_DestroyRenderer(gRenderer);
    if (gWindow != NULL) SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();
}

int main() {
    atexit(free_resources);
    if (!init()) {
        printf("Failed to init! SDL Error: %s\n", SDL_GetError());
    } else if (!loadMedia()) {
        printf("Failed to load media!\n");
    } else {
        loop();
    }
}