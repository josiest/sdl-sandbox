#include <cstdio>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "pi/config/paths.hpp"
#include "pi/systems/system_graph.hpp"
#include "pi/systems/renderer_system.hpp"

#include "gameplay_settings.hpp"
#include "fibonacci_spiral.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[])
{
    pi::system_graph systems;
    if (const auto system_config = pi::load_config("system.yaml")) {
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
    const fib::gameplay_settings settings = fib::load_settings("fibonacci.yaml");
    const fib::spiral_data spiral = fib::load_spiral(settings.spiral);

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
