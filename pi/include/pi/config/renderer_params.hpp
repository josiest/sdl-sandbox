#pragma once
#include "pi/containers/concepts.hpp"
#include "pi/containers/lookup_table.hpp"
#include "pi/config/flags.hpp"

#include <cstdint>
#include <string>
#include <string_view>

#include <optional>
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

static constexpr lookup_table<std::uint32_t, std::string_view, 4>
renderer_flag_names {
    { SDL_RENDERER_SOFTWARE,        "software" },
    { SDL_RENDERER_ACCELERATED,     "accelerated" },
    { SDL_RENDERER_PRESENTVSYNC,    "present-vsync" },
    { SDL_RENDERER_TARGETTEXTURE,   "target-texture" }
};
}

template<>
struct YAML::convert<pi::renderer_params> {

    static YAML::Node encode(const pi::renderer_params& params)
    {
        YAML::Node node;
        if (params.index) { node["index"] = *params.index; }
        if (params.flags) {
            YAML::Node flags;
            pi::write_flags(pi::renderer_flag_names, *params.flags, flags);
            node["flags"] = flags;
        }
        return node;
    }

    static bool decode(const YAML::Node& node, pi::renderer_params& params)
    {
        if (not node.IsMap()) {
            const YAML::Exception error{ node.Mark(), "Map Error" };
            std::printf("%s\n"
                        "Couldn't read renderer parameters because the "
                        "yaml source isn't a map\n", error.what());
            return false;
        }
        if (const auto index = node["index"]) {
            if (int val; YAML::convert<int>::decode(index, val)) {
                params.index = val;
            }
            else {
                const YAML::Exception error{ index.Mark(), "Integer Error" };
                std::printf("%s\n"
                            "Failed to parse renderer index value \"%s\" "
                            "as an integer\n", error.what(), index.Scalar());
            }
        }
        if (const auto flags = node["flags"]) {
            if (flags.IsScalar()) {
                params.flags = pi::read_flag_name(pi::renderer_flag_names,
                                                  flags, 0u);
            }
            if (flags.IsSequence()) {
                if (flags.size() <= 32) {
                    params.flags = pi::read_flag_names(pi::renderer_flag_names,
                                                       flags, 0u);
                }
                else {
                    const YAML::Exception error{ flags.Mark(),
                                                 "Flag Sequence Error" };
                    std::printf("%s\n"
                                "Encountered more than 32 values when reading"
                                "renderer flags, but flags are only 32 bits\n",
                                error.what());
                }
            }
            else {
                const YAML::Exception error{ flags.Mark(), "Sequence Error" };
                std::printf("%s\nCouldn't read renderer flags\n",
                            error.what());
            }
        }
        return true;
    }
};
