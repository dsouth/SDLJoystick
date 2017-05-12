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
static const int TOTAL_BUTTONS = 11;
texture* button[TOTAL_BUTTONS];
texture* button_pressed[TOTAL_BUTTONS];

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
    button[0] = load_texture("../res/button_a.png");
    button_pressed[0] = load_texture("../res/button_a_pressed.png");
    button[1] = load_texture("../res/button_b.png");
    button_pressed[1] = load_texture("../res/button_b_pressed.png");
    button[2] = load_texture("../res/button_x.png");
    button_pressed[2] = load_texture("../res/button_x_pressed.png");
    button[3] = load_texture("../res/button_y.png");
    button_pressed[3] = load_texture("../res/button_y_pressed.png");
    button[4] = load_texture("../res/dpad_up.png");
    button_pressed[4] = load_texture("../res/dpad_up_pressed.png");
    button[5] = load_texture("../res/dpad_down.png");
    button_pressed[5] = load_texture("../res/dpad_down_pressed.png");
    button[6] = load_texture("../res/dpad_left.png");
    button_pressed[6] = load_texture("../res/dpad_left_pressed.png");
    button[7] = load_texture("../res/dpad_right.png");
    button_pressed[7] = load_texture("../res/dpad_right_pressed.png");
    button[8] = load_texture("../res/start.png");
    button_pressed[8] = load_texture("../res/start_pressed.png");
    button[9] = load_texture("../res/select.png");
    button_pressed[9] = load_texture("../res/select_pressed.png");
    button[10] = load_texture("../res/x_box.png");
    button_pressed[10] = load_texture("../res/x_box_pressed.png");

    int result = 0;
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        result = button[i] && button_pressed[i];
    }
    return result;
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
    render_button(button_pressed[0], button[0], s.button_a, x + 75, y + 85);
    render_button(button_pressed[1], button[1], s.button_b, x + 150, y + 47);
    render_button(button_pressed[2], button[2], s.button_x, x, y + 47);
    render_button(button_pressed[3], button[3], s.button_y, x + 75, y);

}

void render_dpad(controller_state s, int x, int y) {
    render_button(button_pressed[4], button[4], s.dpad_up, x + 45, y);
    render_button(button_pressed[5], button[5], s.dpad_down, x + 45, y + 75);
    render_button(button_pressed[6], button[6], s.dpad_left, x, y + 45);
    render_button(button_pressed[7], button[7], s.dpad_right, x + 75, y + 45);
}

void render_bumber(int x1, int y1, Uint8 state) {
    set_color_black();
    SDL_Rect r = {x1, y1, 140, 30};
    if (state == SDL_PRESSED) {
        SDL_RenderFillRect(gRenderer, &r);
    } else {
        SDL_RenderDrawRect(gRenderer, &r);
    }
}

void loop() {

    controller_state s;
    init_controller_state(&s);

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

        render_bumber(5, 5, s.left_bumper);
        render_bumber(150, 5, s.right_bumper);

        render_stick(5, 40, s.left_x_axis, s.left_y_axis, s.left_stick);
        render_stick(150, 40, s.right_x_axis, s.right_y_axis, s.right_stick);
        render_trigger(295, 40, s.left_trigger);
        render_trigger(315, 40, s.right_trigger);

        render_abxy(s, 350, 40);
        render_dpad(s, 55, 210);

        render_button(button_pressed[9], button[9], s.button_back, 260, 220);
        render_button(button_pressed[10], button[10], s.button_x_box, 373, 220);
        render_button(button_pressed[8], button[8], s.button_start, 426, 220);

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
    for (int i = 0; i < TOTAL_BUTTONS; i++) {
        free_texture(button[i]);
        free_texture(button_pressed[i]);
    }
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