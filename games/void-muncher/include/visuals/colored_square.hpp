#pragma once

#include <algorithm>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include <entt/entity/registry.hpp>

#include "pi/algorithm/primitives.hpp"
#include "mechanics/components.hpp"
#include "math/arithmetic.hpp"

inline namespace munch {

inline SDL_Rect
rounded_bbox(const SDL_FPoint& pos, float size)
{
    namespace ranges = std::ranges;
    SDL_Rect rounded;
    auto round_to_int = [](float v) { return static_cast<int>(std::round(v)); };
    SDL_FRect bbox{ pos.x, pos.y, size, size };
    ranges::transform(bbox, ranges::begin(rounded), round_to_int);
    return rounded;
}

inline void
draw_colored_square(SDL_Renderer* renderer,
                    const SDL_FPoint& pos, const SDL_Color& color, float size)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    const SDL_Rect bbox = rounded_bbox(pos, size);
    SDL_RenderFillRect(renderer, &bbox);
}

inline void
draw_all_colored_squares(const entt::registry& entities, SDL_Renderer* renderer)
{
    namespace com = component;
    const auto squares = entities.view<com::position, com::color, com::size>();

    int screen_width = 0;
    int screen_height = 0;
    SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);
    const SDL_FPoint origin_to_pixels{
        pi::divide_by(screen_width, 2.f), pi::divide_by(screen_height, 2.f)
    };

    squares.each([=](const com::position& pos, const com::color& color,
                     const com::size& size){
        const SDL_FPoint position{
            origin_to_pixels.x + pos.value.x, origin_to_pixels.y + pos.value.y,
        };
        draw_colored_square(renderer, position, color.value, size.value);
    });
}
}