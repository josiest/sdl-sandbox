#pragma once
#include <entt/signal/sigh.hpp>
#include <SDL2/SDL_events.h>

inline namespace pi {
struct event_sink{
    void poll();

    inline auto on_quit() { return entt::sink{ quit }; }
    inline auto on_poll() { return entt::sink{ custom_poll }; }
    inline auto on_keydown() { return entt::sink{ keydown }; }
    inline auto on_keyup() { return entt::sink{ keyup }; }

private:
    entt::sigh<void()> quit;
    entt::sigh<void(SDL_Event*)> custom_poll;
    entt::sigh<void(const SDL_KeyboardEvent&)> keydown;
    entt::sigh<void(const SDL_KeyboardEvent&)> keyup;
};
}
