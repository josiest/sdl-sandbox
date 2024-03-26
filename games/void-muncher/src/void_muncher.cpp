#include <cstdio>
#include <cstdlib>

#include <format>
#include <filesystem>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

#include "pi/yaml-config/assets.hpp"
#include "pi/system-graph/system_graph.hpp"
#include "pi/sdl-systems/renderer_system.hpp"
#include "pi/events/event_sink.hpp"

#include "input/keyboard-axis/keyboard_axis.hpp"
#include "colored-squares/colored_square.hpp"
#include "basic-movement/movement.hpp"

#include "world-system/world_system.hpp"
#include "void-muncher/player_controller.hpp"
#include "void-muncher/munchable_system.hpp"
#include "void-muncher/munch_system.hpp"

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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    auto* window = systems.find<pi::window_system>()->window();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    pi::event_sink events;
    events.on_poll().connect<&ImGui_ImplSDL2_ProcessEvent>();

    quit_handler input;
    input.connect_to(events);

    pi::keyboard_axis axis;
    axis.connect_to(events);


    auto& world = *systems.load<munch::world_system>();

    const auto munchable_path = munch::resource_path("munchables").string();
    const auto munchable_config = pi::load_asset<munch::munchable_data>(munchable_path);
    auto& munchables = *systems.load<munch::munchable_system>(munchable_config);

    const auto score_path = munch::resource_path("scoring-system").string();
    const auto score_config = pi::load_asset<munch::score_params>(score_path);
    munch::munch_system munch_system{ score_config };

    const auto muncher_path = munch::resource_path("muncher").string();
    const auto muncher_config = pi::load_asset<munch::muncher_data>(muncher_path);
    auto player = munch::player_controller::create(&world, muncher_config);
    player.connect_to(axis);

    world.init(renderer);
    auto& entities = world.entities;

    std::uint32_t ticks = SDL_GetTicks();
    // (1000 ticks/sec) / (60 frames/sec) = x ticks/frame
    static constexpr std::uint32_t min_ticks = 1000/60;

    bool show_demo_window = false;
    while (not input.has_quit) {
        const std::uint32_t current_ticks = SDL_GetTicks();
        const std::uint32_t delta_ticks = current_ticks-ticks;
        // cap the framerate to go easy on the cpu
        if (delta_ticks < min_ticks) {
            SDL_Delay(min_ticks - delta_ticks);
            continue;
        }
        ticks = current_ticks;

        // ImGui Setup for Current Frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        events.poll();
        munch::update_positions(entities, delta_ticks);
        munchables.update(world, delta_ticks);
        munch_system.munch_or_be_munched(world.entities, player.id);

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        munch::draw_all_colored_squares(entities, renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return EXIT_SUCCESS;
}
