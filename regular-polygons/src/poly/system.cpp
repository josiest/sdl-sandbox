#include "poly/system.hpp"
#include "pi/primitives.hpp"
#include "pi/system.hpp"
#include "pi/config.hpp"

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>
#include <string_view>

#include <memory>
#include <expected>

#include <cstdio>
#include <sstream>

namespace fs = std::filesystem;
namespace msg = YAML::ErrorMsg;

auto error(std::string_view message)
{
    std::stringstream full_message;
    full_message << message << ": " << SDL_GetError();
    return std::unexpected(full_message.str());
}

std::expected<poly::system, std::string>
poly::system::load_from_config(const fs::path& config_path)
{
    std::uint32_t init_flags = SDL_INIT_VIDEO;
    pi::window_params window_params;
    pi::renderer_params renderer_params;

    if (not fs::exists(config_path)) {
        std::printf("Unable to load config because "
                    "the path %s doesn't exist\n", config_path.c_str());
    }
    else if (const YAML::Node config = YAML::LoadFile(config_path.c_str()))  {
        if (not config.IsMap()) {
            std::printf("Couldn't load system config at %s because "
                        "yaml source is not a Map\n", config_path.c_str());
        }
        else {
            if (const auto init_config = config["init"]) {
                pi::read_flags_into<SDL_InitFlags>(init_config, init_flags);
            }
            if (const auto window_config = config["window"]) {
                using as_window = YAML::convert<pi::window_params>;
                if (not as_window::decode(window_config, window_params)) {
                    msg::error(window_config,
                               "encountered errors reading window config");
                }
            }
            if (const auto renderer_config = config["renderer"]) {
                using as_renderer = YAML::convert<pi::renderer_params>;
                if (not as_renderer::decode(renderer_config, renderer_params)) {
                    msg::error(renderer_config,
                               "encountered errors reading renderer config");
                }
            }
        }
    }
    if (SDL_Init(init_flags) != 0) {
        return error("Failed to initialize SDL");
    }
    poly::system system;
    system.window.reset(pi::make_window(window_params));
    if (not system.window) {
        return error("Failed to load SDL Window");
    }
    system.renderer.reset(pi::make_renderer(system.window.get(),
                                            renderer_params));
    if (not system.renderer) {
        return error("Failed to load SDL Renderer");
    }
    return system;
}

poly::system::~system()
{
    if (renderer) { renderer.reset(); }
    if (window) { window.reset(); }
    if (not has_moved) { SDL_Quit(); }
}

poly::system::system(poly::system && tmp)
    : window{ std::move(tmp.window) },
      renderer{ std::move(tmp.renderer) },
      has_moved{ std::exchange(tmp.has_moved, true) }
{
}
