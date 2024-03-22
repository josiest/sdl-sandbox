#include "mechanics/movement.hpp"

void munch::update_positions(entt::registry& entities, std::uint32_t delta_ticks)
{
    namespace component = munch::component;
    auto boxes = entities.view<component::bbox, component::velocity>();
    boxes.each([=](auto& bbox, auto const & velocity) {
        const auto moved_position = move(bbox, velocity, static_cast<float>(delta_ticks));
        bbox.x = moved_position.x;
        bbox.y = moved_position.y;
    });
}
