#pragma once
#include <cmath>
#include <algorithm>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "pi/algorithm/primitives.hpp"
#include "pi/events/event_sink.hpp"
#include "input/keyboard_axis.hpp"

struct muncher {
    inline void connect_to(pi::keyboard_axis& axis)
    {
        axis.on_tick().connect<&muncher::move>(*this);
    }
    inline void disconnect_from(pi::keyboard_axis& axis)
    {
        axis.on_tick().disconnect<&muncher::move>(*this);
    }
    void move(const pi::axis2d& axis)
    {
        bbox.x += speed*axis.first;
        bbox.y += speed*axis.second;
    }
    inline SDL_Rect rounded_bbox() const
    {
        namespace ranges = std::ranges;
        SDL_Rect rounded;
        auto round_to_int = [](float v) -> int { return std::round(v); };
        ranges::transform(bbox, ranges::begin(rounded), round_to_int);
        return rounded;
    }

    float speed = 0.1f;
    SDL_Color color = { 0x0, 0x0, 0x0, 0xff };
    SDL_FRect bbox = { 0.f, 0.f, 20.f, 20.f };
};

inline void draw(SDL_Renderer* renderer, const muncher& player)
{
    SDL_SetRenderDrawColor(renderer, player.color.r, player.color.g,
                                     player.color.b, player.color.a);

    const SDL_Rect bbox = player.rounded_bbox();
    SDL_RenderFillRect(renderer, &bbox);
}
