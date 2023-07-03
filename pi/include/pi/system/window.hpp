#pragma once
#include <optional>
#include <memory>
#include <cstdio>

#include "pi/system/config/window_params.hpp"
#include "pi/system/sdl_deleter.hpp"

#include <SDL2/SDL_window.h>
#include <SDL2/SDL_error.h>

#include <yaml-cpp/yaml.h>
#include <entt/registry.hpp>

inline namespace pi {

struct window_system {
    static constexpr entt::hashed_string name{ "sdl-window" };
    using unique_window = std::unique_ptr<SDL_Window, sdl_deleter>;

    template<std::output_iterator<entt::hashed_string> NameOutput>
    static constexpr NameOutput dependencies(NameOutput into_deps)
    {
        return into_deps;
    }

    inline static bool
    load(entt::registry& subsystems, const YAML::Node& config)
    {
        if (not window_system::can_load(subsystems)) { return false; }

        pi::window_params window_params;
        using as_params = YAML::convert<pi::window_params>;
        if (config) { as_params::decode(config, window_params); }

        auto* window = pi::make_window(window_params);
        if (not window) {
            std::printf("failed to load SDL window: %s\n", SDL_GetError());
            return false;
        }
        subsystems.emplace<window_system>(unique_window{ window });
        return true;
    }

    inline SDL_Window* get_window() const { return window.get(); }
    unique_window window;
};
}
