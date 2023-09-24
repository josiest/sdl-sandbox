#pragma once
#include <cstdint>
#include <utility>

#include <SDL2/SDL_keycode.h>
#include <entt/signal/sigh.hpp>

#include "pi/events/event_sink.hpp"

inline namespace pi {
using axis2d = std::pair<std::int8_t, std::int8_t>;
class keyboard_axis {
public:
    void connect_to(pi::event_sink& sink)
    {
        sink.on_keydown().connect<&keyboard_axis::on_keydown>(*this);
        sink.on_keyup().connect<&keyboard_axis::on_keyup>(*this);
    }
    void disconnect_from(pi::event_sink& sink)
    {
        sink.on_keydown().disconnect<&keyboard_axis::on_keydown>(*this);
        sink.on_keyup().disconnect<&keyboard_axis::on_keyup>(*this);
    }
    void tick(float delta_time) const
    {
        tick_signal.publish(axis, delta_time);
    }
    auto on_tick()
    {
        return entt::sink{ tick_signal };
    }
    SDL_Keycode up = SDLK_w;
    SDL_Keycode left = SDLK_a;
    SDL_Keycode down = SDLK_s;
    SDL_Keycode right = SDLK_d;
private:
    void on_keydown(const SDL_KeyboardEvent& keydown)
    {
        const auto key = keydown.keysym.sym;
        if (key == up) {
            axis.second = -1;
        }
        else if (key == left) {
            axis.first = -1;
        }
        else if (key == down) {
            axis.second = 1;
        }
        else if (key == right) {
            axis.first = 1;
        }
    }
    void on_keyup(const SDL_KeyboardEvent& keydown)
    {
        const auto key = keydown.keysym.sym;
        if (key == up and axis.second < 0) {
            axis.second = 0;
        }
        else if (key == left and axis.first < 0) {
            axis.first = 0;
        }
        else if (key == down and axis.second > 0) {
            axis.second = 0;
        }
        else if (key == right and axis.first > 0) {
            axis.first = 0;
        }
    }
    axis2d axis;
    entt::sigh<void(const axis2d&, float)> tick_signal;
};
}
