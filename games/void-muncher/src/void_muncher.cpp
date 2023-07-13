#include <cstdio>
#include <cstdlib>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "pi/config/paths.hpp"
#include "pi/systems/system_graph.hpp"
#include "pi/systems/renderer_system.hpp"

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

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    bool has_quit = false;
    while (not has_quit) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }
    }
    return EXIT_SUCCESS;
}
