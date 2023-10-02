#pragma once
#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>
#include "mechanics/components.hpp"

inline namespace munch {
namespace component {
struct constant_mover {
    float speed = 10.f;
    SDL_FPoint direction = { 0.f, 0.f };
};
}
inline constexpr SDL_FPoint
move(const component::constant_mover& mover,
     const component::position& pos, float delta_time)
{
    const auto& [speed, direction] = mover;
    const SDL_FPoint velocity{ direction.x * speed, direction.y * speed };
    return SDL_FPoint { pos.value.x + velocity.x * delta_time,
                        pos.value.y + velocity.y * delta_time };
}

void move_constant_movers(entt::registry& entities, float delta_time)
{
    namespace com = component;
    auto movers = entities.view<com::position, com::constant_mover>();
    movers.each([=](auto& pos, auto const & movement_params) {
        pos.value = move(movement_params, pos, delta_time);
    });
}
}