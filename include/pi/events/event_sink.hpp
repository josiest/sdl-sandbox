#pragma once
#include <entt/signal/sigh.hpp>
#include <SDL2/SDL_events.h>

inline namespace pi {
struct event_sink{
    inline void poll()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit.publish();
                break;
            case SDL_KEYDOWN:
                keydown.publish(event.key);
                break;
            case SDL_KEYUP:
                keyup.publish(event.key);
                break;
            default:
                break;
            }
        }
    }
    inline auto on_quit() { return entt::sink{ quit }; }
    inline auto on_keydown() { return entt::sink{ keydown }; }
    inline auto on_keyup() { return entt::sink{ keyup }; }

private:
    entt::sigh<void()> quit;
    entt::sigh<void(const SDL_KeyboardEvent&)> keydown;
    entt::sigh<void(const SDL_KeyboardEvent&)> keyup;
};
}
