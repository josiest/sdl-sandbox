#pragma once
#include "pi/containers/concepts.hpp"
#include "pi/containers/lookup_table.hpp"
#include "pi/config/flags.hpp"
#include "pi/config/error_messages.hpp"
#include "pi/config/sdl_primitives/point.hpp"

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

inline namespace defaults {

constexpr SDL_Point window_position{
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
};
constexpr SDL_Point window_size{ 640, 480 };
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
    const auto name = params.name.value_or("pi window");
    const auto position = params.position.value_or(defaults::window_position);
    const auto size = params.size.value_or(defaults::window_size);
    const auto flags = params.flags.value_or(0u);

    return SDL_CreateWindow(name.c_str(), position.x, position.y,
                                          size.x, size.y, flags);
}

static constexpr lookup_table<std::uint32_t, std::string_view, 20>
window_flag_names{
    { SDL_WINDOW_FULLSCREEN,            "fullscreen" },
    { SDL_WINDOW_FULLSCREEN_DESKTOP,    "fullscreen-desktop" },
    { SDL_WINDOW_OPENGL,                "opengl" },
    { SDL_WINDOW_VULKAN,                "vulkan" },
    { SDL_WINDOW_SHOWN,                 "shown" },
    { SDL_WINDOW_HIDDEN,                "hidden" },
    { SDL_WINDOW_BORDERLESS,            "borderless" },
    { SDL_WINDOW_RESIZABLE,             "resizable" },
    { SDL_WINDOW_MINIMIZED,             "minimized" },
    { SDL_WINDOW_MAXIMIZED,             "maximized" },
    { SDL_WINDOW_INPUT_GRABBED,         "input-grabbed" },
    { SDL_WINDOW_MOUSE_FOCUS,           "mouse-focus" },
    { SDL_WINDOW_FOREIGN,               "foreign" },
    { SDL_WINDOW_ALLOW_HIGHDPI,         "allow-high-dpi" },
    { SDL_WINDOW_MOUSE_CAPTURE,         "mouse-capture" },
    { SDL_WINDOW_ALWAYS_ON_TOP,         "always-on-top" },
    { SDL_WINDOW_SKIP_TASKBAR,          "skip-taskbar" },
    { SDL_WINDOW_UTILITY,               "utility" },
    { SDL_WINDOW_TOOLTIP,               "tooltip" },
    { SDL_WINDOW_POPUP_MENU,            "popup-menu" }
};
}

template<>
struct YAML::convert<pi::window_params> {

    static YAML::Node encode(const pi::window_params& params)
    {
        YAML::Node node;
        if (params.name) { node["name"] = *params.name; }
        if (params.position) {
            node["position"] = *params.position;
        }
        if (params.size) {
            node["size"] = *params.size;
        }
        if (params.flags) {
            YAML::Node flags;
            pi::write_flags(pi::window_flag_names, *params.flags, flags);
            node["flags"] = flags;
        }
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
            if (flags.IsSequence()) {
                if (flags.size() <= 32) {
                    params.flags = pi::read_flag_names(pi::window_flag_names,
                                                       flags, 0u);
                }
                else {
                    const YAML::Exception error{ flags.Mark(),
                                                 "Flag Sequence Error" };
                    std::printf("%s\n"
                                "Tried decoding yaml as a bit-wise integer "
                                "flag, but there are more than 32 flags and "
                                "only 32 bits in an integer\n",
                                error.what());
                }
            }
            else if (flags.IsScalar()) {
                params.flags = pi::read_flag_name(pi::window_flag_names,
                                                  flags, 0u);
            }
            else {
                YAML::Exception error{ flags.Mark(), "Sequence Error" };
                std::printf("%s\n"
                            "Tried decoding yaml as a sequence of bit-wise "
                            "flag names, but the value was not a sequence\n",
                            error.what());
            }
        }
        return true;
    }
};
