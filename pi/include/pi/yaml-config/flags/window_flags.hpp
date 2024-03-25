#pragma once

#include <cstdint>
#include <string_view>

#include "pi/containers/lookup_table.hpp"
#include <yaml-cpp/node/convert.h>
#include <SDL2/SDL_video.h>

template<>
struct YAML::convert<SDL_WindowFlags>{
    static constexpr auto begin() { return flag_names.begin(); }
    static constexpr auto end() { return flag_names.end(); }
    static constexpr auto find(std::string_view name)
    {
        return flag_names.find(name);
    }

    static constexpr pi::lookup_table<std::uint32_t, std::string_view, 20>
    flag_names{
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
};
