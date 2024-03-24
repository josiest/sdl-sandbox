#pragma once
#include <cstdio>
#include <cstdint>
#include <optional>
#include <format>

#include <string_view>

#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>

#include <SDL2/SDL_pixels.h>

#include "pi/reflection/reflect.hpp"
#include "pi/reflection/color.hpp"

#include "pi/yaml-config/color.hpp"
#include "pi/yaml-config/paths.hpp"
#include "pi/yaml-config/meta.hpp"

inline namespace fib {
struct spiral_data {
    SDL_Color initial_color = { 48, 118, 217, 255 };
    SDL_Color final_color = { 219, 0, 66, 255 };
    std::uint8_t num_frames = 9u;
};
}

inline namespace pi {
template<>
auto reflect<fib::spiral_data>()
{
    using namespace entt::literals;
    YAML::convert<SDL_Color>::reflect();

    return entt::meta<fib::spiral_data>()
        .type("fibonacci-spiral"_hs)
        .data<&fib::spiral_data::initial_color>("initial-color"_hs)
        .data<&fib::spiral_data::final_color>("final-color"_hs)
        .data<&fib::spiral_data::num_frames>("num-frames"_hs);
}
}
