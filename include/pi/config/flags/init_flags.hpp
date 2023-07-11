#pragma once

#include <cstdint>
#include <string_view>

#include "pi/containers/lookup_table.hpp"
#include <yaml-cpp/node/convert.h>
#include <SDL2/SDL.h>

typedef enum {
    SDL_INIT_NONE = 0
} SDL_InitFlags;

template<>
struct YAML::convert<SDL_InitFlags> {
    static constexpr auto begin() { return flag_names.begin(); }
    static constexpr auto end() { return flag_names.end(); }
    static constexpr auto find(std::string_view name)
    {
        return flag_names.find(name);
    }

    static constexpr pi::lookup_table<std::uint32_t, std::string_view, 10>
    flag_names{
        { SDL_INIT_TIMER,             "timer" },
        { SDL_INIT_AUDIO,             "audio" },
        { SDL_INIT_VIDEO,             "video" },
        { SDL_INIT_JOYSTICK,          "joystick" },
        { SDL_INIT_HAPTIC,            "haptic" },
        { SDL_INIT_GAMECONTROLLER,    "game-controller" },
        { SDL_INIT_EVENTS,            "events" },
        { SDL_INIT_SENSOR,            "sensor" },
        { SDL_INIT_NOPARACHUTE,       "no-parachute" },
        { SDL_INIT_EVERYTHING,        "everything" },
    };
};
