#include "poly/system.hpp"
#include "pi/paths.hpp"

#include <cstddef>
#include <cstdio>

#include <SDL2/SDL.h>

int main()
{ 
    const auto config_path = pi::resource_path("config.yml");
    auto system = poly::system::load_from_config(config_path);
    if (not system) {
        std::printf("Fatal error: %s\n", system.error().c_str());
        return EXIT_FAILURE;
    }

    bool has_quit = false;
    SDL_Event event;
    while (not has_quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                has_quit = true;
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    has_quit = true;
                }
                }
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
