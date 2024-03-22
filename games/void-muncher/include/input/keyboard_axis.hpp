#pragma once
#include <SDL2/SDL_keycode.h>
#include <entt/signal/sigh.hpp>

#include "pi/events/event_sink.hpp"
#include "input/axis2d.hpp"

inline namespace pi {
class keyboard_axis {
public:
    auto on_axis_changed()
    {
        return entt::sink{ axis_changed_signal };
    }
    void connect_to(pi::event_sink& sink);
private:
    void on_keydown(const SDL_KeyboardEvent& keydown);
    void on_keyup(const SDL_KeyboardEvent& keydown);

public:
    SDL_Keycode up = SDLK_w;
    SDL_Keycode left = SDLK_a;
    SDL_Keycode down = SDLK_s;
    SDL_Keycode right = SDLK_d;
private:
    using axis_sigh = entt::sigh<void(const axis2d8_t& axis)>;

    axis2d8_t axis{ 0, 0 };
    axis_sigh axis_changed_signal;
};
}
