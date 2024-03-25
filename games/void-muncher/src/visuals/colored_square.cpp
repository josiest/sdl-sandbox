#include "visuals/colored_square.hpp"

#include "basic-components/basic_components.hpp"
#include "math/arithmetic.hpp"

void munch::draw_colored_square(SDL_Renderer* renderer, const SDL_Rect& bbox, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &bbox);
}

void munch::draw_all_colored_squares(const entt::registry& entities, SDL_Renderer* renderer)
{
    namespace com = component;
    const auto squares = entities.view<com::bbox, com::color>();

    int screen_width = 0;
    int screen_height = 0;
    SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);
    const SDL_FPoint origin_to_pixels{
        pi::divide_by(screen_width, 2.f), pi::divide_by(screen_height, 2.f)
    };

    squares.each([=](const com::bbox& bbox, const com::color& color) {
        com::bbox centered_box = bbox;
        centered_box.x += origin_to_pixels.x;
        centered_box.y += origin_to_pixels.y;

        draw_colored_square(renderer, static_cast<SDL_Rect>(centered_box), color);
    });
}
