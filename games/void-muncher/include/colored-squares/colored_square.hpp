#pragma once
#include <entt/entity/fwd.hpp>

struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Color;

inline namespace munch {

void draw_colored_square(SDL_Renderer* renderer, const SDL_Rect& bbox, const SDL_Color& color);
void draw_all_colored_squares(const entt::registry& entities, SDL_Renderer* renderer);

}