#include <cstdio>
#include <string_view>
#include <format>
#include <filesystem>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "pi/yaml-config/assets.hpp"
#include "pi/system-graph/system_graph.hpp"
#include "pi/sdl-systems/renderer_system.hpp"

#include "gameplay_settings.hpp"
#include "fibonacci_spiral.hpp"

namespace fs = std::filesystem;
namespace fib {
const fs::path base_dir{ "fibonacci-spiral" };
const fs::path resource_dir = base_dir/"resources";
const fs::path asset_dir = resource_dir/"spirals";

std::string asset_filename(std::string_view asset_name)
{
    return std::format("{}.yaml", asset_name);
}

fs::path config_path(std::string_view config_name)
{
    return fib::resource_dir/asset_filename(config_name);
}

fs::path asset_path(std::string_view asset_name)
{
    return fib::asset_dir/asset_filename(asset_name);
}
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[])
{
    pi::system_graph systems;
    const auto system_path = fib::config_path("system").string();
    if (const auto system_config = pi::load_yaml_resource(system_path)) {
        systems.load<pi::renderer_system>(*system_config);
    }
    else {
        std::printf("%s\nunable to load system config, using defaults\n",
                    system_config.error().c_str());
        systems.load<pi::renderer_system>();
    }
    auto* renderer_system = systems.find<pi::renderer_system>();
    if (not renderer_system) {
        std::printf("Fatal error: unable to load fundamental systems\n");
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = renderer_system->renderer();
    const auto settings_path = fib::config_path("fibonacci").string();
    const auto settings = pi::load_asset<fib::gameplay_settings>(settings_path);

    const auto spiral_path = fib::asset_path(settings.spiral).string();
    const auto spiral = pi::load_asset<fib::spiral_data>(spiral_path);

    bool has_quit = false;
    while (not has_quit) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }
        fib::draw_spiral(renderer, spiral);
        SDL_RenderPresent(renderer);
    }
    return EXIT_SUCCESS;
}
