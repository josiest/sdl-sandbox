#pragma once
#include "pi/config/errors.hpp"
#include "pi/config/primitives.hpp"
#include "pi/config/convert_flags.hpp"
#include "pi/config/flags/window_flags.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <optional>

#include <concepts>
#include <ranges>
#include <iterator>

#include <algorithm>
#include <numeric>
#include <functional>

#include <cstdio>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_rect.h>
#include <yaml-cpp/yaml.h>

inline namespace pi {

namespace defaults::window {

constexpr char name[] = "pi window";
constexpr SDL_Point position{ SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED };
constexpr SDL_Point size{ 640, 480 };
constexpr std::uint32_t flags = 0u;
}

/** A data type for SDL_Window parameters */
struct window_params{
    std::optional<std::string> name = std::nullopt;
    std::optional<SDL_Point> position = std::nullopt;
    std::optional<SDL_Point> size = std::nullopt;
    std::optional<std::uint32_t> flags = std::nullopt;
};

inline SDL_Window* make_window(const window_params& params)
{
    namespace window = defaults::window;

    const auto name = params.name.value_or(window::name);
    const auto position = params.position.value_or(window::position);
    const auto size = params.size.value_or(window::size);
    const auto flags = params.flags.value_or(window::flags);

    return SDL_CreateWindow(name.c_str(), position.x, position.y,
                                          size.x, size.y, flags);
}
}

template<>
struct YAML::convert<pi::window_params> {

    static YAML::Node encode(const pi::window_params& params)
    {
        YAML::Node node;
        if (params.name)        { node["name"] = *params.name; }
        if (params.position)    { node["position"] = *params.position; }
        if (params.size)        { node["size"] = *params.size; }
        if (params.flags)       { node["flags"] = *params.flags; }
        return node;
    }

    static bool decode(const YAML::Node& node, pi::window_params& params)
    {
        namespace msg = YAML::ErrorMsg;

        if (not node.IsMap()) {
            msg::error(node, msg::not_a_map);
            return false;
        }
        if (const auto name = node["name"]) {
            if (name.IsScalar()) {
                params.name = name.as<std::string>();
            }
            else {
                msg::error(name, "failed to read window name");
            }
        }
        using as_point = YAML::convert<SDL_Point>;
        if (const auto position = node["position"]) {
            if (SDL_Point value; as_point::decode(position, value)) {
                params.position = value;
            }
            else {
                msg::error(position, "failed to read window position");
            }
        }
        if (const auto size = node["size"]) {
            if (SDL_Point value; as_point::decode(size, value)) {
                params.size = value;
            }
            else {
                msg::error(size, "failed to read window size");
            }
        }
        if (const auto flags = node["flags"]) {
            std::uint32_t value = 0;
            if (not pi::read_flags_into<SDL_WindowFlags>(flags, value)) {
                msg::error(flags, "encountered errors reading window flags");
            }
            params.flags = value;
        }
        return true;
    }
};
