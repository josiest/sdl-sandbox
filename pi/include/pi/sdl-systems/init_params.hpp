#pragma once
#include <cstdio>
#include <cstdint>
#include <optional>

#include "pi/yaml-config/errors.hpp"
#include "pi/yaml-config/convert_flags.hpp"
#include "pi/yaml-config/flags/init_flags.hpp"

#include <yaml-cpp/yaml.h>

inline namespace pi {

struct init_params{
    std::optional<std::uint32_t> flags = std::nullopt;
};

bool init_sdl(const init_params& params);
}

template<>
struct YAML::convert<pi::init_params>
{
    inline static YAML::Node encode(const pi::init_params& params)
    {
        return pi::encode_flags<SDL_InitFlags>(*params.flags);
    }

    inline static bool decode(const YAML::Node& node, pi::init_params& params)
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
