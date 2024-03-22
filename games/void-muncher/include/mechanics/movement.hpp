#pragma once
#include <cstdint>

#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>

#include "mechanics/components.hpp"

inline namespace munch {
constexpr SDL_FPoint
move(const component::bbox& bbox, const component::velocity& velocity, float delta_ticks)
{
    return SDL_FPoint { bbox.x + (velocity.x * delta_ticks) * .001f,
                        bbox.y + (velocity.y * delta_ticks) * .001f };
}

void update_positions(entt::registry& entities, std::uint32_t delta_ticks);
}