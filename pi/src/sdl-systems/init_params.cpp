#include "pi/sdl-systems/init_params.hpp"
#include <SDL2/SDL.h>

bool pi::init_sdl(const init_params& params)
{
    const auto flags = params.flags.value_or(SDL_INIT_VIDEO);
    return SDL_Init(flags) == 0;
}
