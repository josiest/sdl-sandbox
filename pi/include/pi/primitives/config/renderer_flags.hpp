#pragma once
#include <cstdint>
#include <string_view>

#include "pi/containers/lookup_table.hpp"
#include <yaml-cpp/node/convert.h>
#include <SDL2/SDL_render.h>

template<>
struct YAML::convert<SDL_RendererFlags> {
    static constexpr lookup_table<std::uint32_t, std::string_view, 4>
    flag_names {
        { SDL_RENDERER_SOFTWARE,        "software" },
        { SDL_RENDERER_ACCELERATED,     "accelerated" },
        { SDL_RENDERER_PRESENTVSYNC,    "present-vsync" },
        { SDL_RENDERER_TARGETTEXTURE,   "target-texture" }
    };
    static constexpr auto begin() { return flag_names.begin(); }
    static constexpr auto end() { return flag_names.end(); }
    static constexpr auto find(std::string_view name)
    {
        return flag_names.find(name);
    }
};
