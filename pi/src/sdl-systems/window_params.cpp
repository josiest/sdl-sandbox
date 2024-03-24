#include "pi/sdl-systems/window_params.hpp"
#include <SDL2/SDL_video.h>

SDL_Window* pi::make_window(const pi::window_params& params)
{
    namespace window = defaults::window;

    const auto name = params.name.value_or(window::name);
    const auto position = params.position.value_or(window::position);
    const auto size = params.size.value_or(window::size);
    const auto flags = params.flags.value_or(window::flags);

    return SDL_CreateWindow(name.c_str(), position.x, position.y,
                            size.x, size.y, flags);
}
