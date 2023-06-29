#pragma once
#include "pi/sdl_deleter.hpp"

#include <string>
#include <filesystem>

#include <memory>
#include <expected>

struct SDL_Window;
struct SDL_Renderer;

inline namespace poly {

class system {
public:
    static std::expected<system, std::string>
    load_from_config(const std::filesystem::path& config_path);

    system(const system&) = delete;
    system& operator=(const system&) = delete;
    system& operator=(system &&) = default;

    ~system();
    system(system &&);

    std::unique_ptr<SDL_Window, pi::sdl_deleter> window = nullptr;
    std::unique_ptr<SDL_Renderer, pi::sdl_deleter> renderer = nullptr;
private:
    system() = default;
    bool has_moved = false;
};
}
