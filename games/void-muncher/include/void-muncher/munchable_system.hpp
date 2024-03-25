#pragma once
#include <cstdint>
#include <array>
#include <vector>

#include <random>
#include <cstdint>

#include <iterator>
#include <algorithm>

#include <entt/entity/registry.hpp>
#include <SDL2/SDL_pixels.h>

#include "pi/system-graph/system_graph.hpp"
#include "world-system/world_system.hpp"

inline namespace munch {

namespace component{
struct munchable {};
}

struct munchable_system{

    template<std::output_iterator<entt::id_type> TypeOutput>
    inline static TypeOutput dependencies(TypeOutput into_dependencies)
    {
        namespace ranges = std::ranges;
        return ranges::copy(
            std::array{ entt::type_hash<munch::world_system>::value() },
            into_dependencies).out;
    }

    static munchable_system* load(pi::system_graph & systems);
    static void despawn_any_outside(munch::world_system& world);

    void update(munch::world_system& world, std::uint32_t delta_ticks);
    void spawn(munch::world_system& world) const;

    std::vector<SDL_Color> colors{{0xf3, 0x91, 0x89}, {0xbb, 0x80, 0x82},
                                  {0x6e, 0x75, 0x82}, {0x04, 0x65, 0x82}};

    // how many times to try to spawn per second
    float spawn_frequency = 3.5f;
    // likelihood of spawning
    float spawn_chance = .65f;

    // variance in target angle from spawn to center
    float target_angle_variance = .5f;

    // variance in size from the player
    float size_variance = 0.1f;

    std::uint32_t ticks_since_spawn = std::numeric_limits<std::uint32_t>::max();
};
}

