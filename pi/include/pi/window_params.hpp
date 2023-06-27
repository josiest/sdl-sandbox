#pragma once
// #include "pi/containers.hpp"

#include <array>
#include <utility>

#include <optional>
#include <string>
#include <cstdint>
// #include <string_view>

#include <algorithm>

#include <SDL2/SDL.h>
// #include <yaml-cpp/yaml.h>

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

// static constexpr lookup_table<std::uint32_t, std::string_view, 21>
// window_flag_names{
//     { SDL_WINDOW_FULLSCREEN,            "fullscreen" },
//     { SDL_WINDOW_FULLSCREEN_DESKTOP,    "fullscreen-desktop" },
//     { SDL_WINDOW_OPENGL,                "opengl" },
//     { SDL_WINDOW_VULKAN,                "vulkan" },
//     { SDL_WINDOW_SHOWN,                 "shown" },
//     { SDL_WINDOW_HIDDEN,                "hidden" },
//     { SDL_WINDOW_BORDERLESS,            "borderless" },
//     { SDL_WINDOW_RESIZABLE,             "resizable" },
//     { SDL_WINDOW_MINIMIZED,             "minimized" },
//     { SDL_WINDOW_MAXIMIZED,             "maximized" },
//     { SDL_WINDOW_INPUT_GRABBED,         "input-grabbed" },
//     { SDL_WINDOW_MOUSE_FOCUS,           "mouse-focus" },
//     { SDL_WINDOW_FOREIGN,               "foreign" },
//     { SDL_WINDOW_ALLOW_HIGHDPI,         "allow-high-dpi" },
//     { SDL_WINDOW_MOUSE_CAPTURE,         "mouse-capture" },
//     { SDL_WINDOW_ALWAYS_ON_TOP,         "always-on-top" },
//     { SDL_WINDOW_SKIP_TASKBAR,          "skip-taskbar" },
//     { SDL_WINDOW_UTILITY,               "utility" },
//     { SDL_WINDOW_TOOLTIP,               "tooltip" },
//     { SDL_WINDOW_POPUP_MENU,            "popup-menu" }
// };
// 
// template<typename Container>
// requires can_push_back<Container, std::string>
// void write_window_flags(std::uint32_t flags, Container& into_names)
// {
//     for (const auto [flag, name] : window_flag_names) {
//         if ((flags & flag) == flag) { into_names.push_back(name); }
//     }
// }
// 
// inline constexpr std::uint32_t read_window_flags(YAML::Node& flag_names)
// {
//     namespace ranges = std::ranges;
//     using lookup_elem_t = std::pair<std::uint32_t, std::string_view>;
// 
//     std::uint32_t flags = 0u;
//     for (const auto name : flag_names) {
//         if (not name.IsScalar()) { continue; }
//         const auto result = ranges::find(pi::window_flag_names, name,
//                                          &lookup_elem_t::second);
//         if (not search) { continue; }
//         flags |= search->first;
//     }
//     return flags;
// }
}

// template<>
// struct YAML::convert<pi::window_params> {
// 
//     static YAML::Node encode(const pi::window_params& params)
//     {
//         YAML::Node node;
//         if (params.name) { node["name"] = *params.name; }
//         if (params.x and params.y) {
//             node["position"].push_back(*params.x);
//             node["position"].push_back(*params.y);
//         }
//         if (params.width and params.height) {
//             node["resolution"].push_back(*params.width);
//             node["resolution"].push_back(*params.height);
//         }
//         // if (params.flags) {
//         //     pi::write_window_flags(*params.flags, node["flags"]);
//         // }
//     }
// 
//     static bool decode(const YAML::Node& node, pi::window_params& params)
//     {
//         if (not node.IsMap()) { return true; }
//         if (const auto name = node["name"]; name.IsScalar()) {
//             params.name = name.as<std::string>();
//         }
//         if (const auto position = node["position"];) {
//             if (position.IsSequence() and position.size() == 2) {
//                 try {
//                     params.x = position[0].as<int>();
//                     params.y = position[1].as<int>();
//                 }
//                 catch (...) {
//                 }
//             }
//         }
//         if (const auto resolution = node["resolution"]) {
//             if (resolution.IsSequence() and position.size() == 2) {
//                 try {
//                     params.width = position[0].as<std::uint32_t>();
//                     params.height = position[1].as<std:::uint32_t>();
//                 }
//                 catch (...) {
//                 }
//             }
//         }
//         // if (const auto flags = node["flags"]) {
//         //     if (flags.IsSequence() and flags.size() <= 32) {
//         //         params.flags = pi::read_window_flags(flags)
//         //     }
//         // }
//     }
//     return true;
// };
