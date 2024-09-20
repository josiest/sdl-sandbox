#include "pi/sdl-systems/window_system.hpp"
#include <SDL2/SDL_video.h>
#include <yaml-cpp/yaml.h>

#include "pi/system-graph/system_graph.hpp"
#include "pi/sdl-systems/window_params.hpp"
#include "pi/yaml-config/errors.hpp"

pi::window_system* pi::window_system::load(system_graph& systems)
{
    if (not systems.load<pi::sdl_system>()) { return nullptr; }
    return load(systems, pi::window_params{});
}

pi::window_system* pi::window_system::load(system_graph& systems, const YAML::Node& root)
{
    namespace msg = YAML::ErrorMsg;
    if (not systems.load<pi::sdl_system>(root)) { return nullptr; }

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

pi::window_system* pi::window_system::load(system_graph& systems, const pi::window_params& params)
{
    auto* window = pi::make_window(params);
    if (not window) {
        std::printf("Failed to create a window: %s\n", SDL_GetError());
        return nullptr;
    }
    return &systems.emplace<window_system>(unique_window{ window });
}

SDL_Window* pi::window_system::window() const { return window_handle.get(); }

