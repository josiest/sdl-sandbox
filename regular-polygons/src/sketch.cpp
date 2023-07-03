#include "pi/system.hpp"
#include "pi/config.hpp"
#include "pi/events.hpp"

#include <cstddef>
#include <cstdio>

#include <SDL2/SDL.h>

inline namespace poly {
struct input_manager{
    void connect_to_sink(pi::event_sink& events)
    {
        events.on_quit().connect<&input_manager::on_quit>(*this);
        events.on_keydown().connect<&input_manager::on_keydown>(*this);
    }
    void on_quit() { has_quit = true; }
    void on_keydown(const SDL_KeyboardEvent& event)
    {
        if (event.keysym.sym == SDLK_ESCAPE) { has_quit = true; }
    }
    bool has_quit = false;
};

auto& init_system()
{
    static auto config_path = pi::resource_path("config.yml");
    static auto sys = pi::system::load_from_config(config_path.c_str());
    return sys;
}
}

int main()
{ 
    if (const auto& system = poly::init_system(); not system) {
        std::printf("Fatal error: %s\n", system.error().c_str());
        return EXIT_FAILURE;
    }
    pi::event_sink events;
    poly::input_manager input;
    input.connect_to_sink(events);

    while (not input.has_quit) {
        events.poll();
    }
    return EXIT_SUCCESS;
}
