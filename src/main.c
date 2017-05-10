#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "controller.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Renderer *gRenderer;
SDL_Window *gWindow;
SDL_Joystick *gGameController;

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
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
                        printf("and %d buttons", SDL_JoystickNumButtons(gGameController));
                    }
                }
            }
        }
    }
    return success;
}

int loadMedia() {
    int success = 1;
    return success;
}

void render_stick(int bx, int by, int x, int y) {
    x = x / 512;
    y = y / 512;
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
    SDL_Rect r = {bx, by, 140, 140};
    SDL_RenderDrawRect(gRenderer, &r);
    for (int i = by + 66; i < by + 73; i++) {
        SDL_RenderDrawLine(gRenderer, x + bx + 66,
                           i + y, x + bx + 73, i + y);
    }
}

void render_trigger(int bx, int by, int x) {
    x = (x / 512) + 64;
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
    SDL_Rect r = {bx, by, 18, 129};
    SDL_RenderDrawRect(gRenderer, &r);
    SDL_Rect bar = {bx + 1, 128 - x + by, 16, x};
    SDL_RenderFillRect(gRenderer, &bar);
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

        SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(gRenderer);

        render_stick(5, 5, s.left_x_axis, s.left_y_axis);
        render_stick(150, 5, s.right_x_axis, s.right_y_axis);
        render_trigger(295, 5, s.left_trigger);
        render_trigger(315, 5, s.right_trigger);

        SDL_RenderPresent(gRenderer);
        // update texture here
    }

}

int main() {
    if (!init()) {
        printf("Falied to init! SDL Error: %s\n", SDL_GetError());
    } else if (!loadMedia()) {
        printf("Failed to load media!\n");
    } else {
        loop();
    }
}