#include <SDL2/SDL.h>
#include <cstddef>

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

    auto* window = SDL_CreateWindow("Regular Polygons",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, 
                                    640, 480, 0);
    if (not window) { SDL_Quit(); return EXIT_FAILURE; }

    auto* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (not renderer) { cleanup(window); SDL_Quit(); return EXIT_FAILURE; }

    bool has_quit = false;
    SDL_Event event;
    while (not has_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }
    }
    cleanup(renderer, window); SDL_Quit();
    return EXIT_SUCCESS;
}
