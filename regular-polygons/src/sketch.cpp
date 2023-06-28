#include "pi/paths.hpp"
#include "pi/window_params.hpp"

#include <cstddef>
#include <cstdio>
#include <filesystem>

#include <SDL2/SDL.h>

namespace fs = std::filesystem;

inline namespace pi {
template<typename Resource, typename... Args>
void cleanup(Resource* resource, Args&&... args)
{
    cleanup(resource);
    cleanup(std::forward<Args>(args)...);
}

template<>
inline void cleanup<SDL_Window>(SDL_Window* window)
{
    if (window) { SDL_DestroyWindow(window); }
}
template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer* renderer)
{
    if (renderer) { SDL_DestroyRenderer(renderer); }
}
}

int main()
{ 
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { return EXIT_FAILURE; }

    const auto config_path = pi::resource_path("config.yml");
    pi::window_params window_params;
    if (fs::exists(config_path)) {
        const YAML::Node config = YAML::LoadFile(config_path.c_str());
        if (config.IsMap()) {
            if (const auto window_config = config["window"]) {
                YAML::convert<pi::window_params>
                    ::decode(window_config, window_params);
            }
            else {
                std::printf("Tried reading window settings from config, but "
                            "the project config has no window settings\n");
            }
        }
        else {
            std::printf("Tried reading window settings from config, but "
                        "the project config is not a yaml map\n");
        }
    }
    else {
        std::printf("Tried reading project config at %s, but the path doesn't "
                    "exist\n", config_path.c_str());
    }
    auto* window = pi::make_window(window_params);
    if (not window) { SDL_Quit(); return EXIT_FAILURE; }

    auto* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (not renderer) { cleanup(window); SDL_Quit(); return EXIT_FAILURE; }

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
    cleanup(renderer, window); SDL_Quit();
    return EXIT_SUCCESS;
}
