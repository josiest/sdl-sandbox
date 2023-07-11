#include <cstddef>
#include <cstdio>

#include <SDL2/SDL_render.h>

#include "pi/config/paths.hpp"
#include "pi/systems/system_graph.hpp"
#include "pi/systems/renderer_system.hpp"
#include "fibonacci_spiral.hpp"

int main()
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

    // design parameters
    constexpr SDL_Color blue{ 48, 118, 217, 255 };
    constexpr SDL_Color red{ 219, 0, 66, 255 };
    constexpr std::uint32_t num_frames = 9u;

    bool has_quit = false;
    while (not has_quit) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }
        fib::fibonacci_spiral(blue, red, num_frames)
            .draw_rects_to(renderer);

        SDL_RenderPresent(renderer);
    }
    return EXIT_SUCCESS;
}
