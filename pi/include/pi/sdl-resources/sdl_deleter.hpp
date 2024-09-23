#pragma once
#include <memory>

struct SDL_Window;
struct SDL_Renderer;

inline namespace pi {

// TODO: log verbose instead of printf
struct sdl_deleter {
    void operator()(SDL_Window* window);
    void operator()(SDL_Renderer* renderer);
};

using unique_window = std::unique_ptr<SDL_Window, sdl_deleter>;
using unique_renderer = std::unique_ptr<SDL_Renderer, sdl_deleter>;
}
