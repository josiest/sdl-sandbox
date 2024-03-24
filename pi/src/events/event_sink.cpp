#include "pi/events/event_sink.hpp"
#include <SDL2/SDL_events.h>

void pi::event_sink::poll()
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
