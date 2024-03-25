#include "mechanics/player_controller.hpp"
#include "basic-components/basic_components.hpp"

munch::player_controller
munch::player_controller::create(munch::world_system* world, const
                                 munch::muncher_data& config)
{
    namespace component = munch::component;
    player_controller player;
    player.world = world;

    if (not world) { return player; }
    auto& entities = world->entities;

    player.id = entities.create();
    entities.emplace<component::bbox>(player.id, 0.f, 0.f, config.starting_size);
    entities.emplace<component::color>(player.id, config.color.r, config.color.g, config.color.b);
    entities.emplace<component::velocity>(player.id, 0.f, 0.f);
    entities.emplace<component::dynamic_movement>(player.id, config.speed);

    return player;
}

void munch::player_controller::connect_to(pi::keyboard_axis& axis_delegate)
{
    axis_delegate.on_axis_changed()
                 .connect<&player_controller::orient>(*this);
}

void munch::player_controller::orient(const pi::axis2d8_t& axis) const
{
    if (world and world->entities.valid(id))
    {
        const auto movement = world->entities.get<component::dynamic_movement>(id);
        auto& velocity = world->entities.get<component::velocity>(id);
        const float norm = std::sqrt(velocity.x*velocity.x + velocity.y*velocity.y);
        velocity.x = axis.x * movement.max_speed/norm;
        velocity.y = axis.y * movement.max_speed/norm;
    }
}
