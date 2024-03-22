#pragma once
#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>
#include "mechanics/components.hpp"

inline namespace munch {
inline constexpr SDL_FPoint
move(const component::bbox& bbox, const component::velocity& velocity, float delta_ticks)
{
    return SDL_FPoint { bbox.x + (velocity.x * delta_ticks) * .001f,
                        bbox.y + (velocity.y * delta_ticks) * .001f };
}

void update_positions(entt::registry& entities, std::uint32_t delta_ticks)
{
    auto boxes = entities.view<component::bbox, component::velocity>();
    boxes.each([=](auto& bbox, auto const & velocity) {
        const auto moved_position = move(bbox, velocity, static_cast<float>(delta_ticks));
        bbox.x = moved_position.x;
        bbox.y = moved_position.y;
    });
}
}