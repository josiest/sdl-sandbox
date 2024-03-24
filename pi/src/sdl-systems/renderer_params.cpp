#include "pi/sdl-systems/renderer_params.hpp"

SDL_Renderer* pi::make_renderer(SDL_Window* window, const renderer_params& params)
{
    const auto flags = params.flags.value_or(SDL_RENDERER_ACCELERATED);
    return SDL_CreateRenderer(window, -1, flags);
}
