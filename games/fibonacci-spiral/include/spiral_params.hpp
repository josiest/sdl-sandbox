#pragma once
#include <cstdio>
#include <cstdint>
#include <optional>

#include <string_view>

#include <SDL2/SDL_pixels.h>
#include <yaml-cpp/yaml.h>

#include "pi/config/color.hpp"
#include "pi/config/paths.hpp"

inline namespace fib {
struct fibonacci_spiral {
    SDL_Color initial_color = { 48, 118, 217, 255 };
    SDL_Color final_color = { 219, 0, 66, 255 };
    std::uint8_t num_frames = 9u;
};
fibonacci_spiral load_spiral(std::string_view spiral_path);
}

template<>
struct YAML::convert<fib::fibonacci_spiral> {
    static YAML::Node encode(const fib::fibonacci_spiral& params)
    {
        YAML::Node node;
        node["initial-color"] = params.initial_color;
        node["final-color"] = params.final_color;
        node["num-frames"] = params.num_frames;
        return node;
    }
    static bool decode(const YAML::Node& node, fib::fibonacci_spiral& params)
    {
        namespace msg = YAML::ErrorMsg;
        if (not node)
        {
            std::printf("%s\n", msg::invalid_node);
            return false;
        }
        using as_color = YAML::convert<SDL_Color>;
        if (const auto initial_config = node["initial-color"]) {
            if (not as_color::decode(initial_config, params.initial_color)) {
                msg::error(initial_config, "failed to read initial color");
            }
        }
        if (const auto final_config = node["final-color"]) {
            if (not as_color::decode(final_config, params.final_color)) {
                msg::error(final_config, "failed to read final color");
            }
        }
        using as_int = YAML::convert<std::uint8_t>;
        if (const auto num_config = node["num-frames"]) {
            if (not as_int::decode(num_config, params.num_frames)) {
                msg::error(num_config, "failed to read num frames");
            }
        }
        return true;
    }
};

fib::fibonacci_spiral fib::load_spiral(std::string_view spiral_path)
{
    fib::fibonacci_spiral spiral;

    using as_spiral = YAML::convert<fib::fibonacci_spiral>;
    const auto config = pi::load_config("spiral.yaml");
    if (config) {
        if (as_spiral::decode(*config, spiral)) { return spiral; }
    }
    else {
        std::printf("%s\n", config.error());
    }
    std::printf("unable to load spiral config, using defaults\n");
    return spiral;
}
