#include "basic-movement/movement.hpp"
#include <entt/entity/registry.hpp>

void munch::update_velocities(entt::registry& entities, std::uint32_t delta_ticks)
{
    auto boxes = entities.view<component::velocity, component::acceleration,
                               component::dynamic_movement>();

    const float delta_ms = static_cast<float>(delta_ticks);
    boxes.each([=](component::velocity& velocity, const component::acceleration& acceleration,
                                                  const component::dynamic_movement & movement_params) {

        velocity.x += (acceleration.x * delta_ms) * .001f;
        velocity.y += (acceleration.y * delta_ms) * .001f;

        const float current_speed = std::sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
        const SDL_FPoint friction{ -movement_params.friction_constant * velocity.x/current_speed,
                                   -movement_params.friction_constant * velocity.y/current_speed };

        velocity.x += friction.x;
        velocity.y += friction.y;

        if (current_speed > movement_params.max_speed) {
            const float clamp_ratio = movement_params.max_speed / current_speed;
            velocity.x *= clamp_ratio;
            velocity.y *= clamp_ratio;
        }
        else if (current_speed <= movement_params.min_speed) {
            velocity.x = 0.f;
            velocity.y = 0.f;
        }
    });
}

void munch::update_positions(entt::registry& entities, std::uint32_t delta_ticks)
{
    auto boxes = entities.view<component::bbox, component::velocity>();
    const float delta_ms = static_cast<float>(delta_ticks);
    boxes.each([=](component::bbox& bbox, const component::velocity & velocity) {
        bbox.x += (velocity.x * delta_ms) * .001f;
        bbox.y += (velocity.y * delta_ms) * .001f;
    });
}
