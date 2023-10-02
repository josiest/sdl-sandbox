#pragma once
#include <utility>
#include <cstdint>
#include <entt/signal/sigh.hpp>

inline namespace pi {

template<typename Field>
requires std::is_arithmetic_v<Field>
struct axis2d {
    Field x;
    Field y;

    explicit constexpr operator SDL_FPoint() const
    {
        return SDL_FPoint{ static_cast<float>(x), static_cast<float>(y) };
    }

    explicit constexpr operator SDL_Point() const
    {
        return SDL_Point{ static_cast<int>(x), static_cast<int>(y) };
    }
};

template<typename Field> requires std::is_arithmetic_v<Field>
using axis2d_tick_signal = entt::sigh<void(const axis2d<Field>&, float)>;

using axis2d8_t = axis2d<std::int8_t>;
using axis2d8_tick_signal = axis2d_tick_signal<std::int8_t>;
}
