#pragma once
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

inline namespace pi {

struct sdl_deleter {
    inline void operator()(SDL_Window* window)
    {
        SDL_DestroyWindow(window);
    }
    inline void operator()(SDL_Renderer* renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
};
}
