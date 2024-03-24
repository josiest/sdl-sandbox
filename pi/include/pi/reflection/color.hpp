#pragma once

#include <SDL2/SDL_pixels.h>
#include <entt/core/fwd.hpp>
#include <entt/meta/factory.hpp>
#include "pi/reflection/reflect.hpp"

#include <cstdio>

inline namespace pi {
template<>
inline auto reflect<SDL_Color>()
{
    using namespace entt::literals;
    return entt::meta<SDL_Color>()
        .type("sdl::color"_hs)
        .data<&SDL_Color::r>("red"_hs)
        .data<&SDL_Color::g>("green"_hs)
        .data<&SDL_Color::b>("blue"_hs)
        .data<&SDL_Color::a>("alpha"_hs);
}
}
