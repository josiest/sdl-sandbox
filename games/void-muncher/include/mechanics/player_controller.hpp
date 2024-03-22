#pragma once
#include <cmath>
#include <algorithm>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "pi/algorithm/primitives.hpp"
#include "pi/events/event_sink.hpp"

#include "pi/meta/reflect.hpp"
#include "pi/meta/color.hpp"
#include "pi/config/color.hpp"

#include "input/keyboard_axis.hpp"
#include "mechanics/components.hpp"
#include "movement/constant_mover.hpp"
#include "game/world_system.hpp"

inline namespace munch {
struct muncher_data {
    SDL_Color color{ 0x0, 0x0, 0x0, 0xff };
    float starting_size = 20.f;
    float speed = 200.f;
};

namespace component {
struct dynamic_movement{
    float max_speed = 100.f;
};
}
}

inline namespace pi {
template<>
[[maybe_unused]] auto reflect<muncher_data>()
{
    using namespace entt::literals;
    YAML::convert<SDL_Color>::reflect();

    return entt::meta<munch::muncher_data>()
        .type("muncher-settings"_hs)
        .data<&muncher_data::color>("color"_hs)
        .data<&muncher_data::starting_size>("starting-size"_hs)
        .data<&muncher_data::speed>("speed"_hs);
}
}

inline namespace munch {
class player_controller {
public:
    static player_controller
    create(munch::world_system* world,
           const muncher_data& config)
    {
        player_controller player;
        player.world = world;

        if (not world) { return player; }
        auto& entities = world->entities;

        player.id = entities.create();
        entities.emplace<component::bbox>(player.id, 0.f, 0.f, config.starting_size);
        entities.emplace<component::color>(player.id, config.color.r, config.color.g,
                                                      config.color.b);
        entities.emplace<component::velocity>(player.id, 0.f, 0.f);
        entities.emplace<component::dynamic_movement>(player.id, config.speed);

        return player;
    }

    inline void connect_to(pi::keyboard_axis& axis_delegate)
    {
        axis_delegate.on_axis_changed()
                     .connect<&player_controller::orient>(*this);
    }

    inline void orient(const pi::axis2d8_t& axis) const
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

    munch::world_system* world = nullptr;
    entt::entity id{ 0 };
private:
    player_controller() = default;
};

}