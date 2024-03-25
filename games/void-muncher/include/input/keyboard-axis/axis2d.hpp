#pragma once
#include <type_traits>
#include <cstdint>
#include <SDL2/SDL_rect.h>

inline namespace pi {

template<typename Field>
requires std::is_arithmetic_v<Field>
struct axis2d {
    explicit constexpr operator SDL_FPoint() const
    {
        return SDL_FPoint{ static_cast<float>(x), static_cast<float>(y) };
    }

    explicit constexpr operator SDL_Point() const
    {
        return SDL_Point{ static_cast<int>(x), static_cast<int>(y) };
    }

    Field x;
    Field y;
};

using axis2d8_t = axis2d<std::int8_t>;
}
