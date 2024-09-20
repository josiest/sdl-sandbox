#include "pi/sdl-resources/sdl_handle.hpp"
#include "pi/sdl-resources/sdl_params.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char * argv[])
{
    pi::sdl_params init_params;
    auto sdl_handle = pi::sdl_handle::init(init_params.flags.value_or(SDL_INIT_VIDEO));
    if (not sdl_handle) { std::cout << "Unable to init sdl: " << sdl_handle.error() << "\n"; }
    return EXIT_SUCCESS;
}