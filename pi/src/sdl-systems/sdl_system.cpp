#include "pi/sdl-systems/sdl_system.hpp"

#include <cstdio>  // std::printf
#include <utility> // std::exchange

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "pi/system-graph/system_graph.hpp"
#include "pi/sdl-resources/sdl_params.hpp"
#include "pi/yaml-config/errors.hpp"

pi::sdl_system::sdl_system(sdl_system && tmp) noexcept
    : should_quit{ std::exchange(tmp.should_quit, false) }
{
}

pi::sdl_system& pi::sdl_system::operator=(sdl_system && tmp) noexcept
{
    should_quit = std::exchange(tmp.should_quit, false);
    return *this;
}

pi::sdl_system::~sdl_system()
{
    if (should_quit) {
        std::printf("quit sdl\n");
        SDL_Quit(); should_quit = false;
    }
}

std::expected<pi::sdl_system, std::string>
pi::sdl_system::init(std::uint32_t init_flags)
{
    if (SDL_Init(init_flags) != 0) {
        return std::unexpected{ SDL_GetError() };
    }
    return sdl_system{};
}

pi::sdl_system* pi::sdl_system::load(system_graph& systems)
{
    return load(systems, pi::sdl_params{});
}

pi::sdl_system* pi::sdl_system::load(system_graph& systems, const YAML::Node& root)
{
    namespace msg = YAML::ErrorMsg;

    pi::sdl_params params;
    if (not root or not root.IsMap()) { return load(systems, params); }

    const auto config = root["init"];
    if (not config) { return load(systems, params); }

    using as_params = YAML::convert<pi::sdl_params>;
    if (not as_params::decode(config, params)) {
        msg::error(config, "encountered errors reading init flags");
    }
    return load(systems, params);
}

pi::sdl_system* pi::sdl_system::load(system_graph& systems, const pi::sdl_params& params)
{
    if (not pi::init_sdl(params)) {
        std::printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return nullptr;
    }
    return &systems.emplace<sdl_system>(sdl_system{});
}
