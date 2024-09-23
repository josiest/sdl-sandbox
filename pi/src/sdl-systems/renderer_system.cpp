#include "pi/system-graph/system_graph.hpp"
#include "pi/sdl-resources/renderer_params.hpp"
#include "pi/sdl-systems/renderer_system.hpp"

pi::renderer_system* pi::renderer_system::load(pi::system_graph& systems)
{
    auto* window_sys = systems.load<window_system>();
    if (not window_sys) { return nullptr; }

    return load(systems, window_sys->window(), pi::renderer_params{});
}

pi::renderer_system* pi::renderer_system::load(pi::system_graph& systems, const YAML::Node& root)
{
    namespace msg = YAML::ErrorMsg;
    auto* window_sys = systems.load<pi::window_system>(root);
    if (not window_sys) { return nullptr; }
    auto* window = window_sys->window();

    pi::renderer_params params;
    if (not root or not root.IsMap()) {
        return load(systems, window, params);
    }
    const auto& config = root["renderer"];
    if (not config) { return load(systems, window, params); }

    using as_params = YAML::convert<pi::renderer_params>;
    if (not as_params::decode(config, params)) {
        msg::error(config, "unable to read renderer params");
    }
    return load(systems, window, params);
}

pi::renderer_system* pi::renderer_system::load(pi::system_graph& systems, SDL_Window* window,
                                               const pi::renderer_params& params)
{
    auto* renderer = pi::make_renderer(window, params);
    if (not renderer) {
        std::printf("Failed to create a renderer: %s\n", SDL_GetError());
        return nullptr;
    }
    return &systems.emplace<renderer_system>(unique_renderer{ renderer });
}

SDL_Renderer* pi::renderer_system::renderer() { return renderer_handle.get(); }
