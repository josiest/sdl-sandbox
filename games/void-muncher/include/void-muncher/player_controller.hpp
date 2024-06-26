#pragma once
#include <SDL2/SDL_pixels.h>
#include <entt/entity/registry.hpp>

#include "pi/algorithm/primitives.hpp"
#include "pi/events/event_sink.hpp"

#include "pi/reflection/reflect.hpp"
#include "pi/reflection/color.hpp"
#include "pi/yaml-config/color.hpp"

#include "input/keyboard-axis/keyboard_axis.hpp"
#include "world-system/world_system.hpp"

inline namespace munch {
struct muncher_data {
    SDL_Color color{ 0x0, 0x0, 0x0, 0xff };
    float starting_size = 20.f;
    float acceleration_constant = 100.f;
    float friction_constant = .2f;
    float min_speed = 1.f;
    float max_speed = 200.f;
};
}

inline namespace pi {
template<> inline auto reflect<muncher_data>()
{
    using namespace entt::literals;
    YAML::convert<SDL_Color>::reflect();

    return entt::meta<munch::muncher_data>()
        .type("muncher-settings"_hs)
        .data<&muncher_data::color>("color"_hs)
        .data<&muncher_data::starting_size>("starting-size"_hs)
        .data<&muncher_data::acceleration_constant>("acceleration-constant"_hs)
        .data<&muncher_data::friction_constant>("friction-constant"_hs)
        .data<&muncher_data::min_speed>("min-speed"_hs)
        .data<&muncher_data::max_speed>("max-speed"_hs);
}
}

inline namespace munch {
class player_controller {
public:
    static player_controller create(munch::world_system* world, const muncher_data& config);

    void connect_to(pi::keyboard_axis& axis_delegate);
    void orient(const pi::axis2d8_t& axis) const;

    munch::world_system* world = nullptr;
    entt::entity id{ 0 };
private:
    player_controller() = default;
};

}