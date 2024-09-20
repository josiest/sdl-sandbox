#include "pi/sdl-resources/sdl_handle.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char * argv[])
{
    auto sdl_handle = pi::sdl_handle::init();
    if (not sdl_handle) { std::cout << "Unable to init sdl: " << sdl_handle.error() << "\n"; }
    return EXIT_SUCCESS;
}