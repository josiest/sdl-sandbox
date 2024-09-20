#include "pi/sdl-resources/sdl_params.hpp"
#include <SDL2/SDL.h>

bool pi::init_sdl(const sdl_params& params)
{
    const auto flags = params.flags.value_or(SDL_INIT_VIDEO);
    return SDL_Init(flags) == 0;
}
