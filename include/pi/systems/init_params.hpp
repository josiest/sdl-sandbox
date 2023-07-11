#pragma once
#include <cstdio>
#include <cstdint>
#include <optional>

#include "pi/config/errors.hpp"
#include "pi/config/convert_flags.hpp"
#include "pi/config/flags/init_flags.hpp"

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

inline namespace pi {

struct init_params{
    std::optional<std::uint32_t> flags = std::nullopt;
};

inline bool init_sdl(const init_params& params)
{
    const auto flags = params.flags.value_or(SDL_INIT_VIDEO);
    return SDL_Init(flags) == 0;
}
}

template<>
struct YAML::convert<pi::init_params>
{
    static YAML::Node encode(const pi::init_params& params)
    {
        return pi::encode_flags<SDL_InitFlags>(*params.flags);
    }
    static bool decode(const YAML::Node& node, pi::init_params& params)
    {
        namespace msg = YAML::ErrorMsg;
        if (not node) {
            std::printf("%s\n", msg::invalid_node);
            return false;
        }
        std::uint32_t value = SDL_INIT_VIDEO;
        if (not pi::read_flags_into<SDL_InitFlags>(node, value)) {
            msg::error(node, "encountered errors reading renderer flags");
        }
        params.flags = value;
        return true;

    }
};
