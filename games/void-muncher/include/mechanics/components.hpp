#pragma once
#include <cstdint>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>

inline namespace munch {
namespace component {

struct bbox {
    constexpr operator SDL_FRect() const;
    explicit constexpr operator SDL_Rect() const;

    float x, y, size;
};

struct velocity {
    constexpr operator SDL_FPoint() const;
    float x, y;
};

struct color {
    constexpr operator SDL_Color() const;
    std::uint8_t r, g, b;
};
}
}

constexpr munch::component::bbox::operator SDL_FRect() const
{
    return SDL_FRect{ x, y, size, size };
}

constexpr munch::component::bbox::operator SDL_Rect() const
{
    return SDL_Rect{ static_cast<int>(x), static_cast<int>(y),
                     static_cast<int>(size), static_cast<int>(size) };
}

constexpr munch::component::velocity::operator SDL_FPoint() const
{
    return SDL_FPoint{ x, y };
}

constexpr munch::component::color::operator SDL_Color() const
{
    return SDL_Color{ r, g, b };
}
