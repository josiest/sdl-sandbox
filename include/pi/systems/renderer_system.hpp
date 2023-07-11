#pragma once
#include <iterator>
#include <algorithm>

#include <array>
#include <string_view>

#include <memory>
#include <cstdio>

#include <SDL2/SDL_render.h>
#include <entt/core/fwd.hpp>

#include "pi/systems/system_graph.hpp"
#include "pi/systems/window_system.hpp"
#include "pi/systems/sdl_deleter.hpp"

#include "pi/systems/renderer_params.hpp"
#include "pi/config/errors.hpp"

inline namespace pi {
class renderer_system {
public:
    using unique_renderer = std::unique_ptr<SDL_Renderer, sdl_deleter>;

    template<std::output_iterator<entt::id_type> TypeOutput>
    inline static TypeOutput dependencies(TypeOutput into_types)
    {
        namespace ranges = std::ranges;
        return ranges::copy(
                std::array{ entt::type_hash<window_system>::value() },
                into_types).out;
    }
    inline static renderer_system*
    load(pi::system_graph& systems)
    {
        return load(systems, pi::renderer_params{});
    }
    inline static renderer_system*
    load(pi::system_graph& systems, const YAML::Node& root)
    {
        namespace msg = YAML::ErrorMsg;

        pi::renderer_params params;
        if (not root or not root.IsMap()) { return load(systems, params); }

        const auto& config = root["renderer"];
        if (not config) { return load(systems, params); }

        using as_params = YAML::convert<pi::renderer_params>;
        if (not as_params::decode(config, params)) {
            msg::error(config, "unable to read renderer params");
        }
        return load(systems, params);
    }
    inline static renderer_system*
    load(pi::system_graph& systems, const pi::renderer_params& params)
    {
        auto* window_sys = systems.load<window_system>();
        if (not window_sys) { return nullptr; }

        auto* renderer = pi::make_renderer(window_sys->window(), params);
        if (not renderer) {
            std::printf("Failed to create a renderer: %s\n", SDL_GetError());
        }
        return &systems.emplace<renderer_system>(unique_renderer{ renderer });
    }

    SDL_Renderer* renderer() { return renderer_handle.get(); }
    unique_renderer renderer_handle;
};
}
