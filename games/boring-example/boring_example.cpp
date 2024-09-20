#include "pi/containers/lookup_table.hpp"
#include "pi/sdl-systems/sdl_system.hpp"

#include <cstdlib>
#include <string_view>

#include <iostream>

enum class test_enum {
    hello, goodbye, what
};

pi::lookup_table<test_enum, std::string_view, 3> test{
    { test_enum::hello, "world" },
    { test_enum::goodbye, "cruel world" },
    { test_enum::what, "the fuck" }
};

int main(int argc, char * argv[])
{
    auto sdl_handle = pi::sdl_system::init();
    if (not sdl_handle) { std::cout << "Unable to init sdl: " << sdl_handle.error() << "\n"; }
    else { std::cout << "SDL initialized successfully\n"; }
    return EXIT_SUCCESS;
}