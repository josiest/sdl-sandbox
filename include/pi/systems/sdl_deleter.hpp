#pragma once
#include <cstdio>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

inline namespace pi {

// TODO: log verbose instead of printf
struct sdl_deleter {
    inline void operator()(SDL_Window* window)
    {
        std::printf("destroying window\n");
        SDL_DestroyWindow(window);
    }
    inline void operator()(SDL_Renderer* renderer)
    {
        std::printf("destroying renderer\n");
        SDL_DestroyRenderer(renderer);
    }
};
}
