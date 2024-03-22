#pragma once
#include <cstdint>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>

inline namespace munch {
namespace component {

struct bbox {
    operator SDL_FRect() const
    {
        return SDL_FRect{ x, y, size, size };
    }

    explicit operator SDL_Rect() const
    {
        return SDL_Rect{ static_cast<int>(x), static_cast<int>(y),
                         static_cast<int>(size), static_cast<int>(size) };
    }

    float x, y, size;
};

struct velocity {
    operator SDL_FPoint() const
    {
        return SDL_FPoint{ x, y };
    }

    float x, y;
};

struct color {
    operator SDL_Color() const
    {
        return SDL_Color{ r, g, b };
    }

    std::uint8_t r, g, b;
};
}
}
