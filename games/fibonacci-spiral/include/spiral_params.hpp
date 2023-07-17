#pragma once
#include <cstdio>
#include <cstdint>
#include <optional>

#include <string_view>

#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>

#include <SDL2/SDL_pixels.h>
#include <yaml-cpp/yaml.h>

#include "pi/meta/reflect.hpp"
#include "pi/meta/color.hpp"

#include "pi/config/color.hpp"
#include "pi/config/paths.hpp"
#include "pi/config/meta.hpp"

inline namespace fib {
struct fibonacci_spiral {
    SDL_Color initial_color = { 48, 118, 217, 255 };
    SDL_Color final_color = { 219, 0, 66, 255 };
    std::uint8_t num_frames = 9u;
};
fibonacci_spiral load_spiral(std::string_view spiral_path);
}

inline namespace pi {
template<>
auto reflect<fib::fibonacci_spiral>()
{
    using namespace entt::literals;
    YAML::convert<SDL_Color>::reflect();

    return entt::meta<fib::fibonacci_spiral>()
        .type("fibonacci-spiral"_hs)
        .data<&fib::fibonacci_spiral::initial_color>("initial-color"_hs)
        .data<&fib::fibonacci_spiral::final_color>("final-color"_hs)
        .data<&fib::fibonacci_spiral::num_frames>("num-frames"_hs);
}
}

fib::fibonacci_spiral fib::load_spiral(std::string_view spiral_path)
{
    fib::fibonacci_spiral spiral;
    using as_spiral = YAML::convert<fib::fibonacci_spiral>;
    const auto config = pi::load_config("spiral.yaml");
    if (config) {
        if (pi::config::decode(*config, spiral)) { return spiral; }
    }
    else {
        std::printf("%s\n", config.error());
    }
    std::printf("unable to load spiral config, using defaults\n");
    return spiral;
}
