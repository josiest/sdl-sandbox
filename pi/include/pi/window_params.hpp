#pragma once
#include "pi/containers.hpp"

#include <cstdint>
#include <string>
// #include <string_view>

#include <optional>
#include <utility>
#include <array>

#include <algorithm>

#include <cstdio>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

inline namespace pi {

/** A data type for SDL_Window parameters */
struct window_params{
    std::optional<std::string> name = std::nullopt;
    std::optional<int> x = std::nullopt;
    std::optional<int> y = std::nullopt;
    std::optional<std::uint32_t> width = std::nullopt;
    std::optional<std::uint32_t> height = std::nullopt;
    std::optional<std::uint32_t> flags = std::nullopt;
};

inline SDL_Window* make_window(const window_params& params)
{
    const auto name = params.name.value_or("pi window");
    const auto x = params.x.value_or(SDL_WINDOWPOS_UNDEFINED);
    const auto y = params.y.value_or(SDL_WINDOWPOS_UNDEFINED);
    const int width = params.width.value_or(640u);
    const int height = params.height.value_or(480u);
    const auto flags = params.flags.value_or(0);

    return SDL_CreateWindow(name.c_str(), x, y, width, height, flags);
}

static constexpr lookup_table<std::uint32_t, std::string_view, 20>
window_flag_names{{
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
}};

template<typename Container>
requires can_push_back<Container, std::string>
void write_window_flags(std::uint32_t flags, Container& into_names)
{
    for (const auto [flag, name] : window_flag_names) {
        if ((flags & flag) == flag) {
            into_names.push_back(std::string{ name });
        }
    }
}

inline constexpr std::uint32_t read_window_flags(const YAML::Node& node)
{
    namespace ranges = std::ranges;
    using lookup_elem_t = std::pair<std::uint32_t, std::string_view>;

    if (node.IsScalar()) {
        const auto search = ranges::find(pi::window_flag_names,
                                         node.as<std::string>(),
                                         &lookup_elem_t::second);
        if (not search) { return 0u; }
        return search->first;
    }
    if (not node.IsSequence()) { return 0u; }

    std::uint32_t flags = 0u;
    for (const auto name : node) {
        if (not name.IsScalar()) { continue; }
        const auto search = ranges::find(pi::window_flag_names,
                                         name.as<std::string>(),
                                         &lookup_elem_t::second);
        if (not search) { continue; }
        std::printf("Using flag %s\n", search->second);
        flags |= search->first;
    }
    return flags;
}
}

template<>
struct YAML::convert<pi::window_params> {

    static YAML::Node encode(const pi::window_params& params)
    {
        YAML::Node node;
        if (params.name) { node["name"] = *params.name; }
        if (params.x and params.y) {
            node["position"].push_back(*params.x);
            node["position"].push_back(*params.y);
        }
        if (params.width and params.height) {
            node["resolution"].push_back(*params.width);
            node["resolution"].push_back(*params.height);
        }
        if (params.flags) {
            YAML::Node flags;
            pi::write_window_flags(*params.flags, flags);
            node["flags"] = flags;
        }
        return node;
    }

    static bool decode(const YAML::Node& node, pi::window_params& params)
    {
        if (not node.IsMap()) {
            const YAML::Exception error{ node.Mark(), "Map Error" };
            std::printf("%s\n",
                        "Tried decoding yaml as SDL window params, but the "
                        "yaml source is not a map\n", error.what());
            return true;
        }
        if (const auto name = node["name"]) {
            if (name.IsScalar()) {
                params.name = name.as<std::string>();
            }
            else {
                const YAML::Exception error{ name.Mark(), "Scalar Error" };
                std::printf("%s\n",
                            "Tried decoding window name from yaml, but value "
                            "is not scalar\n", error.what());
            }
        }
        if (const auto position = node["position"]) {
            if (position.IsSequence() and position.size() == 2) {
                try {
                    params.x = position[0].as<int>();
                    params.y = position[1].as<int>();
                }
                catch (YAML::Exception error) {
                    std::printf("%s\n"
                                "Tried decoding yaml as an integer position "
                                "but the sequence values weren't integers\n",
                                error.what());
                }
            }
            else {
                YAML::Exception error{ position.Mark(), "Sequence Error" };
                std::printf("%s\n"
                            "Tried decoding yaml as integer position but the "
                            "yaml source was either not a sequence or didn't "
                            "have exactly two values\n",
                            error.what());
            }
        }
        if (const auto resolution = node["resolution"]) {
            if (resolution.IsSequence() and resolution.size() == 2) {
                try {
                    params.width = resolution[0].as<int>();
                    params.height = resolution[1].as<int>();
                }
                catch (YAML::Exception error) {
                    std::printf("%s\n"
                                "Tried decoding yaml as an integer pair "
                                "but the sequence values weren't integers\n",
                                error.what());
                }
            }
            else {
                YAML::Exception error{ resolution.Mark(), "Sequence Error" };
                std::printf("%s\n"
                            "Tried decoding yaml as an integer pair but the "
                            "yaml source was either not a sequence or didn't "
                            "have exactly two values\n",
                            error.what());
            }
        }
        if (const auto flags = node["flags"]) {
            if (flags.IsSequence()) {
                if (flags.size() <= 32) {
                    params.flags = pi::read_window_flags(flags);
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
                params.flags = pi::read_window_flags(flags);
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
