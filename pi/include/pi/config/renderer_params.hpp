#pragma once
#include "pi/containers/concepts.hpp"
#include "pi/containers/lookup_table.hpp"

#include <cstdint>
#include <string>
#include <string_view>

#include <optional>

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

static constexpr lookup_table<std::uint32_t, std::string_view, 4> {
    { SDL_RENDERER_SOFTWARE,        "software" },
    { SDL_RENDERER_ACCELERATED,     "accelerated" },
    { SDL_RENDERER_PRESENTVSYNC,    "present-vsync" },
    { SDL_RENDERER_TARGETTEXTURE,   "target-texture" }
};
}
