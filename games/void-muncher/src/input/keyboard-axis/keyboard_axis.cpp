#include "input/keyboard-axis/keyboard_axis.hpp"

void pi::keyboard_axis::connect_to(pi::event_sink& sink)
{
    sink.on_keydown().connect<&keyboard_axis::on_keydown>(*this);
    sink.on_keyup().connect<&keyboard_axis::on_keyup>(*this);
}

void pi::keyboard_axis::on_keydown(const SDL_KeyboardEvent& keydown)
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

void pi::keyboard_axis::on_keyup(const SDL_KeyboardEvent& keydown)
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
