#pragma once
#include <cstdint>
#include <utility>

#include <SDL2/SDL_keycode.h>
#include <entt/signal/sigh.hpp>

#include "pi/events/event_sink.hpp"
#include "input/axis2d.hpp"

inline namespace pi {
class keyboard_axis {
public:
    void connect_to(pi::event_sink& sink)
    {
        sink.on_keydown().connect<&keyboard_axis::on_keydown>(*this);
        sink.on_keyup().connect<&keyboard_axis::on_keyup>(*this);
    }

    auto on_axis_changed()
    {
        return entt::sink{ axis_changed_signal };
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
            axis.y = -1;
        }
        else if (key == left) {
            axis.x = -1;
        }
        else if (key == down) {
            axis.y = 1;
        }
        else if (key == right) {
            axis.x = 1;
        }
        axis_changed_signal.publish(axis);
    }
    void on_keyup(const SDL_KeyboardEvent& keydown)
    {
        const auto key = keydown.keysym.sym;
        if (key == up and axis.y < 0) {
            axis.y = 0;
        }
        else if (key == left and axis.x < 0) {
            axis.x = 0;
        }
        else if (key == down and axis.y > 0) {
            axis.y = 0;
        }
        else if (key == right and axis.x > 0) {
            axis.x = 0;
        }
        axis_changed_signal.publish(axis);
    }
    axis2d8_t axis{ 0, 0 };

    using axis_sigh = entt::sigh<void(const axis2d8_t& axis)>;
    axis_sigh axis_changed_signal;
};
}
