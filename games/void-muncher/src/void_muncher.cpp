#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "pi/config/paths.hpp"
#include "pi/systems/system_graph.hpp"
#include "pi/systems/renderer_system.hpp"

#include "pi/events/event_sink.hpp"
#include "input/keyboard_axis.hpp"



void print_axis(const pi::axis2d& axis)
{
    std::printf("[%d, %d]\n", axis.first, axis.second);
}

struct quit_handler {
    void connect_to(pi::event_sink& sink)
    {
        sink.on_quit().connect<&quit_handler::on_quit>(*this);
    }
    void disconnect_from(pi::event_sink& sink)
    {
        sink.on_quit().disconnect<&quit_handler::on_quit>(*this);
    }
    void on_quit() { has_quit = true; }
    bool has_quit = false;
};

int main()
{
    pi::system_graph systems;
    if (const auto system_config = pi::load_config("system.yaml")) {
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
    axis.on_tick().connect<&print_axis>();

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    while (not input.has_quit) {
        events.poll();
        axis.tick();
    }
    return EXIT_SUCCESS;
}
