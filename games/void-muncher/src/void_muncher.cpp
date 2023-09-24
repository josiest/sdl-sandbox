#include <cstdio>
#include <cstdlib>

#include <format>
#include <filesystem>

#include <SDL2/SDL_render.h>

#include "pi/config/assets.hpp"
#include "pi/systems/system_graph.hpp"
#include "pi/systems/renderer_system.hpp"

#include "pi/events/event_sink.hpp"
#include "input/keyboard_axis.hpp"
#include "muncher.hpp"

namespace fs = std::filesystem;

inline namespace munch {
const fs::path resource_dir{ "resources" };

inline fs::path resource_path(std::string_view asset_name)
{
    return resource_dir/std::format("{}.yaml", asset_name);
}
}

struct quit_handler {
    void connect_to(pi::event_sink& sink)
    {
        sink.on_quit().connect<&quit_handler::on_quit>(*this);
    }
    void on_quit() { has_quit = true; }
    bool has_quit = false;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[])
{
    pi::system_graph systems;
    const auto system_path = munch::resource_path("system").string();
    if (const auto system_config = pi::load_yaml_resource(system_path)) {
        systems.load<pi::renderer_system>(*system_config);
    }
    else {
        std::printf("%s\n"
                    "unable to load system config, using defaults\n",
                    system_config.error().c_str());
        systems.load<pi::renderer_system>();
    }
    SDL_Renderer* renderer;
    if (auto* renderer_system = systems.find<pi::renderer_system>()) {
        renderer = renderer_system->renderer();
    }
    else {
        std::printf("Fatal error: unable to load fundamental systems\n");
        return EXIT_FAILURE;
    }

    pi::event_sink events;

    quit_handler input;
    input.connect_to(events);

    pi::keyboard_axis axis;
    axis.connect_to(events);

    const auto muncher_path = munch::resource_path("muncher").string();
    const auto muncher_config = pi::load_asset<munch::muncher_data>(muncher_path);
    munch::muncher player{ muncher_config };
    player.connect_to(axis);
    player.reset(renderer);

    std::uint32_t ticks = SDL_GetTicks();
    while (not input.has_quit) {
        const std::uint32_t current_ticks = SDL_GetTicks();
        const float delta_time = static_cast<float>(current_ticks-ticks)/100.f;
        ticks = current_ticks;

        events.poll();
        axis.tick(delta_time);

        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(renderer);

        draw(renderer, player);
        SDL_RenderPresent(renderer);
    }
    return EXIT_SUCCESS;
}
