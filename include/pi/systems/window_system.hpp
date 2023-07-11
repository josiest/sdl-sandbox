#pragma once
#include <iterator>
#include <algorithm>

#include <array>
#include <string_view>

#include <memory>
#include <cstdio>

#include <SDL2/SDL_video.h>
#include <entt/core/fwd.hpp>

#include "pi/systems/system_graph.hpp"
#include "pi/systems/init_system.hpp"
#include "pi/systems/sdl_deleter.hpp"

#include "pi/systems/window_params.hpp"
#include "pi/config/errors.hpp"

inline namespace pi {

struct window_system {
    using unique_window = std::unique_ptr<SDL_Window, sdl_deleter>;

    template<std::output_iterator<entt::id_type> TypeOutput>
    inline static TypeOutput dependencies(TypeOutput into_dependencies)
    {
        namespace ranges = std::ranges;
        return ranges::copy(
                std::array{ entt::type_hash<init_system>::value() },
                into_dependencies).out;
    }
    inline static window_system*
    load(system_graph& systems)
    {
        return load(systems, pi::window_params{});
    }
    inline static window_system*
    load(system_graph& systems, const YAML::Node& root)
    {
        namespace msg = YAML::ErrorMsg;

        pi::window_params params;
        if (not root or not root.IsMap()) { return load(systems, params); }

        const auto& config = root["window"];
        if (not config) { return load(systems, params); }

        using as_params = YAML::convert<pi::window_params>;
        if (not as_params::decode(config, params)) {
            msg::error(config, "unable to read window params");
        }
        return load(systems, params);
    }
    inline static window_system*
    load(system_graph& systems, const pi::window_params& params)
    {
        if (not systems.load<init_system>()) { return nullptr; }
        auto* window = pi::make_window(params);
        if (not window) {
            std::printf("Failed to create a window: %s\n", SDL_GetError());
            return nullptr;
        }
        return &systems.emplace<window_system>(unique_window{ window });
    }
    SDL_Window* window() { return window_handle.get(); }
    unique_window window_handle;
};
}
