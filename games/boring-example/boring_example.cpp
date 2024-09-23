#include "pi/sdl-resources/sdl_handle.hpp"
#include "pi/sdl-resources/sdl_params.hpp"
#include "pi/sdl-resources/sdl_deleter.hpp"
#include "pi/sdl-resources/window_params.hpp"
#include "pi/sdl-resources/renderer_params.hpp"
#include "pi/yaml-config/assets.hpp"

#include <cstdlib>
#include <string>
#include <iostream>
#include <filesystem>

int main(int argc, char * argv[])
{
    pi::sdl_params init_params;
    auto sdl_handle = pi::sdl_handle::init(init_params.flags.value_or(SDL_INIT_VIDEO));
    if (not sdl_handle) {
        std::cout << "Unable to init sdl: " << sdl_handle.error() << "\n";
        return EXIT_FAILURE;
    }

    namespace fs = std::filesystem;
    fs::path const base_dir = SDL_GetBasePath();
    auto const config_path = base_dir/"config.yml";

    auto window_settings = pi::load_asset<pi::window_params>(config_path.string());
    pi::unique_window window_handle{ pi::make_window(window_settings) };

    pi::renderer_params renderer_settings{};
    pi::unique_renderer renderer_handle{ pi::make_renderer(window_handle.get(), renderer_settings) };
    auto* renderer = renderer_handle.get();

    bool has_quit = false;
    while (not has_quit) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                has_quit = true;
            }
        }
        SDL_Rect render_frame{ 0, 0, 0, 0 };
        SDL_GetRendererOutputSize(renderer, &render_frame.w, &render_frame.h);

        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderFillRect(renderer, &render_frame);
        SDL_RenderPresent(renderer);
    }
    return EXIT_SUCCESS;
}