#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include <entt/entity/registry.hpp>

inline namespace munch {

void draw_colored_square(SDL_Renderer* renderer, const SDL_Rect& bbox, const SDL_Color& color);
void draw_all_colored_squares(const entt::registry& entities, SDL_Renderer* renderer);

}