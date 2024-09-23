#include "pi/sdl-resources/sdl_deleter.hpp"

#include <cstdio>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>

void pi::sdl_deleter::operator()(SDL_Window* window)
{
    std::printf("destroying window\n");
    SDL_DestroyWindow(window);
}

void pi::sdl_deleter::operator()(SDL_Renderer* renderer)
{
    std::printf("destroying renderer\n");
    SDL_DestroyRenderer(renderer);
}
