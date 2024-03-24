#pragma once

struct SDL_Window;
struct SDL_Renderer;

inline namespace pi {

// TODO: log verbose instead of printf
struct sdl_deleter {
    void operator()(SDL_Window* window);
    void operator()(SDL_Renderer* renderer);
};
}
