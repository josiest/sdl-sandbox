#pragma once
#include <cstdio>
#include <cstdint>
#include <optional>

#include "pi/config/errors.hpp"
#include "pi/config/convert_flags.hpp"
#include "pi/config/flags/renderer_flags.hpp"

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL_render.h>

inline namespace pi {

struct renderer_params{
    std::optional<std::uint32_t> flags = std::nullopt;
};

inline SDL_Renderer* make_renderer(SDL_Window* window,
                                   const renderer_params& params)
{
    const auto flags = params.flags.value_or(SDL_RENDERER_ACCELERATED);
    return SDL_CreateRenderer(window, -1, flags);
}
}

template<>
struct YAML::convert<pi::renderer_params> {

    static YAML::Node encode(const pi::renderer_params& params)
    {
        return pi::encode_flags<SDL_RendererFlags>(*params.flags);
    }
    static bool decode(const YAML::Node& node, pi::renderer_params& params)
    {
        namespace msg = YAML::ErrorMsg;
        if (not node) {
            std::printf("%s\n", msg::invalid_node);
            return false;
        }
        std::uint32_t value = SDL_RENDERER_ACCELERATED;
        if (not pi::read_flags_into<SDL_RendererFlags>(node, value)) {
            msg::error(node, "encountered errors reading renderer flags");
        }
        params.flags = value;
        return true;
    }
};
