#pragma once
#include <cstdint>
#include <optional>

#include "pi/config/error_messages.hpp"
#include "pi/config/sdl_primitives/renderer_flags.hpp"
#include "pi/config/sdl_primitives/flags.hpp"

#include <SDL2/SDL_render.h>

inline namespace pi {

struct renderer_params{
    std::optional<int> index = std::nullopt;
    std::optional<std::uint32_t> flags = std::nullopt;
};

inline SDL_Renderer* make_renderer(SDL_Window* window,
                                   const renderer_params& params)
{
    const int index = params.index.value_or(-1);
    const auto flags = params.flags.value_or(SDL_RENDERER_ACCELERATED);
    return SDL_CreateRenderer(window, index, flags);
}

}

template<>
struct YAML::convert<pi::renderer_params> {

    static YAML::Node encode(const pi::renderer_params& params)
    {
        YAML::Node node;
        if (params.index) { node["index"] = *params.index; }
        if (params.flags) {
            node["flags"] = pi::encode_flags<SDL_RendererFlags>(*params.flags);
        }
        return node;
    }
    static bool decode(const YAML::Node& node, pi::renderer_params& params)
    {
        namespace msg = YAML::ErrorMsg;
        if (not node.IsMap()) {
            msg::error(node, msg::not_a_map);
            return false;
        }
        using as_int = YAML::convert<int>;
        if (const auto index = node["index"]) {
            if (int val; as_int::decode(index, val)) {
                params.index = val;
            }
            else {
                msg::error(index, msg::not_an_integer);
            }
        }
        if (const auto flags = node["flags"]) {
            std::uint32_t value = SDL_RENDERER_ACCELERATED;
            if (not pi::read_flags_into<SDL_RendererFlags>(flags, value)) {
                msg::error(flags, "encountered errors reading renderer flags");
            }
            params.flags = value;
        }
        return true;
    }
};
