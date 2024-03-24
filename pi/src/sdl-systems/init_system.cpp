#include "pi/sdl-systems/init_system.hpp"

#include <cstdio>  // std::printf
#include <utility> // std::exchange

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "pi/system-graph/system_graph.hpp"
#include "pi/sdl-systems/init_params.hpp"
#include "pi/yaml-config/errors.hpp"

pi::init_system::init_system(init_system && tmp)
    : should_quit{ std::exchange(tmp.should_quit, false) }
{
}

pi::init_system& pi::init_system::operator=(init_system && tmp)
{
    should_quit = std::exchange(tmp.should_quit, false);
    return *this;
}

pi::init_system::~init_system()
{
    if (should_quit) {
        std::printf("quit sdl\n");
        SDL_Quit(); should_quit = false;
    }
}

pi::init_system* pi::init_system::load(system_graph& systems)
{
    return load(systems, pi::init_params{});
}

pi::init_system* pi::init_system::load(system_graph& systems, const YAML::Node& root)
{
    namespace msg = YAML::ErrorMsg;

    pi::init_params params;
    if (not root or not root.IsMap()) { return load(systems, params); }

    const auto config = root["init"];
    if (not config) { return load(systems, params); }

    using as_params = YAML::convert<pi::init_params>;
    if (not as_params::decode(config, params)) {
        msg::error(config, "encountered errors reading init flags");
    }
    return load(systems, params);
}

pi::init_system* pi::init_system::load(system_graph& systems, const pi::init_params& params)
{
    if (not pi::init_sdl(params)) {
        std::printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return nullptr;
    }
    return &systems.emplace<init_system>(init_system{});
}
