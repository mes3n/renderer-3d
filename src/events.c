#include "events.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

bool should_close_flag = false;
bool should_close() { return should_close_flag; }

void poll_events() {
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
            default:
                break;
            }
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
