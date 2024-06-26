#include "void-muncher/player_controller.hpp"
#include "basic-components/basic_components.hpp"
#include "basic-movement/movement.hpp"

munch::player_controller
munch::player_controller::create(munch::world_system* world, const
                                 munch::muncher_data& config)
{
    player_controller player;
    player.world = world;

    if (not world) { return player; }
    auto& entities = world->entities;

    player.id = entities.create();
    world->player_id = player.id;
    entities.emplace<component::bbox>(player.id, 0.f, 0.f, config.starting_size);
    entities.emplace<component::color>(player.id, config.color.r, config.color.g, config.color.b);
    entities.emplace<component::velocity>(player.id, 0.f, 0.f);
    entities.emplace<component::acceleration>(player.id, 0.f, 0.f);
    entities.emplace<component::dynamic_movement>(player.id,
                                                  config.acceleration_constant, config.friction_constant,
                                                  config.min_speed, config.max_speed);

    return player;
}

void munch::player_controller::connect_to(pi::keyboard_axis& axis_delegate)
{
    axis_delegate.on_axis_changed()
                 .connect<&player_controller::orient>(*this);
}

void munch::player_controller::orient(const pi::axis2d8_t& axis) const
{
    const float norm = std::sqrt(static_cast<float>(axis.x*axis.x + axis.y*axis.y));
    if (world and world->entities.valid(id))
    {
        const auto movement = world->entities.get<component::dynamic_movement>(id);
        const float acc = movement.acceleration_constant/std::max(norm, 1.f);
        world->entities.replace<component::acceleration>(id, static_cast<float>(axis.x)*acc,
                                                             static_cast<float>(axis.y)*acc);
    }
}
