#include "void-muncher/player_controller.hpp"
#include "basic-components/basic_components.hpp"

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
    const float norm = std::sqrt(static_cast<float>(axis.x*axis.x + axis.y*axis.y));
    if (world and world->entities.valid(id))
    {
        const auto movement = world->entities.get<component::dynamic_movement>(id);
        const float speed = movement.max_speed/std::max(norm, 1.f);
        world->entities.replace<component::velocity>(id, axis.x*speed, axis.y*speed);
    }
}

void munch::player_controller::munch_or_be_munched()
{
    if (not world or not world->entities.valid(id)) { return; }
    const SDL_FRect player_bbox = world->entities.get<component::bbox>(id);

    auto munchables = world->entities.view<component::bbox>();
    for (const auto & [munchable, munchable_bbox] : munchables.each()) {

        const SDL_FRect munchable_b_rect = munchable_bbox;
        if (munchable == id) { continue; }
        if (not SDL_HasIntersectionF(&munchable_b_rect, &player_bbox)) { continue; }
        if (player_bbox.w > munchable_bbox.size) {
            world->entities.destroy(munchable);
        }
        else {
            world->entities.destroy(id);
        }
    }
}