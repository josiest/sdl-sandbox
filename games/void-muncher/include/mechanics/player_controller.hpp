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

inline namespace munch {
struct muncher_data {
    SDL_Color color{ 0x0, 0x0, 0x0, 0xff };
    float starting_size = 20.f;
    float speed = 10.f;
};
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
    create(const std::weak_ptr<entt::registry>& in_entities,
           const muncher_data& config)
    {
        player_controller player;
        player.entity_context = in_entities;

        auto entities = player.entity_context.lock();
        if (not entities) { return player; }

        player.id = entities->create();
        entities->emplace<component::position>(player.id, SDL_FPoint{ 0.f, 0.f });
        entities->emplace<component::size>(player.id, config.starting_size);
        entities->emplace<component::color>(player.id, config.color);
        entities->emplace<component::constant_mover>(
            player.id, config.speed, SDL_FPoint{ 0.f, 0.f });

        return player;
    }

    inline void connect_to(pi::keyboard_axis& axis_delegate)
    {
        axis_delegate.on_axis_changed()
                     .connect<&player_controller::orient>(*this);
    }

    inline void orient(const pi::axis2d8_t& axis) const
    {
        if (auto entities = entity_context.lock();
            entities and entities->valid(id))
        {
            auto& mover = entities->get<component::constant_mover>(id);
            mover.direction = static_cast<SDL_FPoint>(axis);
        }
    }

    std::weak_ptr<entt::registry> entity_context;
    entt::entity id{ 0 };
private:
    player_controller() = default;
};

}