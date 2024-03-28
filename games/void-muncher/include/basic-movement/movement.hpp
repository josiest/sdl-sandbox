#pragma once
#include <cstdint>

#include <SDL2/SDL_rect.h>
#include <entt/entity/fwd.hpp>

#include "basic-components/basic_components.hpp"

inline namespace munch {

namespace component {
struct acceleration {
    constexpr operator SDL_FPoint() const;
    float x, y;
};

struct velocity {
    constexpr operator SDL_FPoint() const;
    float x, y;
};

struct dynamic_movement {
    float acceleration_constant = 100.f;
    float friction_constant = .2f;
    float min_speed = 1.f;
    float max_speed = 200.f;
};
}

void update_velocities(entt::registry & entities, std::uint32_t delta_ticks);
void update_positions(entt::registry & entities, std::uint32_t delta_ticks);
}

constexpr munch::component::acceleration::operator SDL_FPoint() const
{
    return SDL_FPoint{ x, y };
}

constexpr munch::component::velocity::operator SDL_FPoint() const
{
    return SDL_FPoint{ x, y };
}
