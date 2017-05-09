#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
    RIGHT_BUMPTER = 5,
    LEFT_STICK = 6,
    RIGHT_STICK = 7,
    START = 8,
    BACK = 9,
    X_BOX = 10,
    DPAD_UP = 11,
    DPAD_DOWN = 12,
    DPAD_LEFT = 13,
    DPATH_RIGHT = 14,
};


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


void loop() {

    const int STICK_DEADZONE = 8000;

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_JOYAXISMOTION) {
                if (e.jaxis.axis == LEFT_STICK_X_AXIS
                    || e.jaxis.axis == LEFT_STICK_Y_AXIS
                    || e.jaxis.axis == RIGHT_STICK_X_AXIS
                    || e.jaxis.axis == RIGHT_STICK_Y_AXIS) {
                    if (e.jaxis.value < -STICK_DEADZONE ||
                        e.jaxis.value > STICK_DEADZONE) {
                        printf("%d joystick moving to value %d\n", e.jaxis.axis, e.jaxis.value);
                    }
                } else if (e.jaxis.axis == LEFT_TRIGGER_AXIS
                        || e.jaxis.axis == RIGHT_TRIGGER_AXIS) {
                    printf("%d trigger moving to value %d\n", e.jaxis.axis, e.jaxis.value);
                }
            } else if (e.type == SDL_JOYBUTTONDOWN) {
                printf("%d button pressed\n", e.jbutton.button);
            } else if (e.type == SDL_JOYBUTTONUP) {
                printf("%d button released\n", e.jbutton.button);
            }
        }
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