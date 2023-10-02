#pragma once
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>

inline namespace munch {
namespace component {
struct position {
    SDL_FPoint value;
};
struct size {
    float value;
};
struct color {
    SDL_Color value;
};
}
}
