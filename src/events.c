#include "events.h"
#include "camera.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>

bool should_close_flag = false;
bool should_close() { return should_close_flag; }

double mouse_sensitivity = 0.2;
void init_events() { SDL_SetRelativeMouseMode(SDL_TRUE); }

void poll_events(Camera *camera, const double dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            should_close_flag = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                should_close_flag = true;
                break;
            case SDLK_w:
                camera_move(camera, CAMERA_FORWARD, dt);
                break;
            case SDLK_s:
                camera_move(camera, CAMERA_BACKWARD, dt);
                break;
            case SDLK_a:
                camera_move(camera, CAMERA_LEFT, dt);
                break;
            case SDLK_d:
                camera_move(camera, CAMERA_RIGHT, dt);
                break;
            case SDLK_SPACE:
                camera_move(camera, CAMERA_UP, dt);
                break;
            case SDLK_LCTRL:
                camera_move(camera, CAMERA_DOWN, dt);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            camera_turn(camera,
                        (double)event.motion.xrel * dt * mouse_sensitivity,
                        (double)event.motion.yrel * dt * mouse_sensitivity);
            break;
        default:
            break;
        }
    }
}

double tick() {
    static unsigned long ticks_ms = 0;
    unsigned long now = SDL_GetTicks64();
    unsigned long result = now - ticks_ms;
    ticks_ms = now;
    return (double)result / 1000.0;
}
