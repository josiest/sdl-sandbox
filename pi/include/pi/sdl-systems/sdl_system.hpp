#pragma once
#include <cstdint>
#include <expected>
#include <string>
#include <SDL2/SDL.h>

namespace YAML {
class Node;
}

inline namespace pi {
class system_graph;
struct init_params;

class sdl_system {
public:
#pragma region Rule of Five
    sdl_system(const sdl_system&) = delete;
    sdl_system& operator=(const sdl_system&) = delete;

    sdl_system(sdl_system && tmp) noexcept;
    sdl_system& operator=(sdl_system && tmp) noexcept;

    ~sdl_system();
#pragma endregion
    static std::expected<sdl_system, std::string>
    init(std::uint32_t init_flags = SDL_INIT_VIDEO);

    static sdl_system* load(system_graph& systems);
    static sdl_system* load(system_graph& systems, const YAML::Node& root);
    static sdl_system* load(system_graph& systems, const pi::init_params& params);
private:
    sdl_system() = default;
    bool should_quit = true;
};
}
