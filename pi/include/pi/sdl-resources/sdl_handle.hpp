#pragma once
#include <cstdint>
#include <string>
#include <expected>

#include <SDL2/SDL.h>

inline namespace pi {

class sdl_handle {
public:
#pragma region Rule of Five
    sdl_handle(const sdl_handle&) = delete;
    sdl_handle& operator=(const sdl_handle&) = delete;

    sdl_handle(sdl_handle && tmp) noexcept;
    sdl_handle& operator=(sdl_handle && tmp) noexcept;

    ~sdl_handle();
#pragma endregion
    static std::expected<sdl_handle, std::string>
    init(std::uint32_t init_flags = SDL_INIT_VIDEO);
private:
    sdl_handle() = default;
    bool should_quit = true;
};
}