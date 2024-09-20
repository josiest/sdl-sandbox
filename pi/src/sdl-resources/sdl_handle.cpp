#include "pi/sdl-resources/sdl_handle.hpp"

#include <cstdio>  // std::printf
#include <utility> // std::exchange

#include <SDL2/SDL.h>

pi::sdl_handle::sdl_handle(sdl_handle && tmp) noexcept
    : should_quit{ std::exchange(tmp.should_quit, false) }
{
}

pi::sdl_handle& pi::sdl_handle::operator=(sdl_handle && tmp) noexcept
{
    should_quit = std::exchange(tmp.should_quit, false);
    return *this;
}

pi::sdl_handle::~sdl_handle()
{
    if (should_quit) {
        std::printf("quit sdl\n");
        SDL_Quit(); should_quit = false;
    }
}

std::expected<pi::sdl_handle, std::string>
pi::sdl_handle::init(std::uint32_t init_flags)
{
    if (SDL_Init(init_flags) != 0) {
        return std::unexpected{ SDL_GetError() };
    }
    std::printf("sdl initialized\n");
    return sdl_handle{};
}
