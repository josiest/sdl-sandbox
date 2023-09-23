#pragma once
#include <cstdio>
#include <cstdint>
#include <optional>

#include <string_view>

#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>

#include <SDL2/SDL_pixels.h>

#include "pi/meta/reflect.hpp"
#include "pi/meta/color.hpp"

#include "pi/config/color.hpp"
#include "pi/config/paths.hpp"
#include "pi/config/meta.hpp"

inline namespace fib {
struct spiral_data {
    SDL_Color initial_color = { 48, 118, 217, 255 };
    SDL_Color final_color = { 219, 0, 66, 255 };
    std::uint8_t num_frames = 9u;
};
spiral_data load_spiral(std::string_view spiral_path);
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

fib::spiral_data fib::load_spiral(std::string_view spiral_path)
{
    pi::reflect<fib::spiral_data>();
    const auto config = pi::load_config(spiral_path);
    fib::spiral_data spiral;
    if (config) {
        if (pi::config::decode(*config, spiral)) { return spiral; }
    }
    else {
        std::printf("%s\n", config.error().c_str());
    }
    std::printf("unable to load spiral config, using defaults\n");
    return spiral;
}
