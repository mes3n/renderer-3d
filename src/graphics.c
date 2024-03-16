#include "graphics.h"

#include <SDL2/SDL.h>
#include <stdio.h>

SDL_Window *sdl_window;
SDL_Renderer *sdl_renderer;

const int sdl_scale = 8;

int init_graphics(const int image_width, const int image_height) {
    SDL_CreateWindowAndRenderer(image_width * sdl_scale,
                                image_height * sdl_scale, 0, &sdl_window,
                                &sdl_renderer);
    if (sdl_window == NULL || sdl_renderer == NULL) {
        fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);

    return 0;
}

static inline Vec3 linear_to_gamma(Vec3 v) {
    return vec3_from(sqrt(v.x), sqrt(v.y), sqrt(v.z));
}

void set_pixel(int x, int y, Vec3 rgb) {
    rgb = linear_to_gamma(rgb);
    int r = (int)(255.999 * rgb.x);
    int g = (int)(255.999 * rgb.y);
    int b = (int)(255.999 * rgb.z);
    SDL_Rect rect = (SDL_Rect){
        .x = x * sdl_scale,
        .y = y * sdl_scale,
        .w = sdl_scale,
        .h = sdl_scale,
    };

    SDL_SetRenderDrawColor(sdl_renderer, r, g, b, 255);
    SDL_RenderFillRect(sdl_renderer, &rect);
}

void update_render(void) {
    SDL_RenderPresent(sdl_renderer);
    SDL_RenderClear(sdl_renderer);
}

void stop_graphics(void) {
    // do {
    //     SDL_PumpEvents();
    // } while (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE] == 0);
    //
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}
