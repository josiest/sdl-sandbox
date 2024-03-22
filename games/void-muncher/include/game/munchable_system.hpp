#pragma once
#include <memory>
#include <random>

#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

#include "pi/systems/system_graph.hpp"

#include "mechanics/components.hpp"
#include "movement/constant_mover.hpp"
#include "game/world_system.hpp"

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

    inline static munchable_system* load(pi::system_graph & systems);
    inline void init(munch::world_system& world);
    inline void update(munch::world_system& world, std::uint32_t delta_ticks);
    inline void spawn(munch::world_system& world) const;
    inline void despawn_any_outside(munch::world_system& world) const;

    SDL_FRect world_bounds{ 0, 0, 800, 600 };

    // how many times to try to spawn per second
    float spawn_frequency = 3.5f;
    // likelihood of spawning
    float spawn_chance = .65f;

    // variance in target angle from spawn to center
    float target_angle_variance = .5f;

    std::uint32_t ticks_since_spawn = std::numeric_limits<std::uint32_t>::max();
};
}

munch::munchable_system* munch::munchable_system::load(pi::system_graph & systems)
{
    auto* world = systems.load<munch::world_system>();
    if (not world) {
        return nullptr;
    }
    return &systems.emplace<munch::munchable_system>();
}

void munch::munchable_system::init(munch::world_system & world)
{
    world_bounds.x = -static_cast<float>(world.bounds.x)/2;
    world_bounds.y = -static_cast<float>(world.bounds.y)/2;
    world_bounds.w =  static_cast<float>(world.bounds.x);
    world_bounds.h =  static_cast<float>(world.bounds.y);
}

void munch::munchable_system::update(munch::world_system & world, std::uint32_t delta_ticks)
{
    if (ticks_since_spawn < std::numeric_limits<std::uint32_t>::max() - delta_ticks) {
        ticks_since_spawn += delta_ticks;
    }
    // (1000 ticks/sec) / (x hz) = y ticks
    if (ticks_since_spawn <= 1000/spawn_frequency) {
        return;
    }
    despawn_any_outside(world);
    std::discrete_distribution<> coin_toss({ 1.f-spawn_chance, spawn_chance });
    if (coin_toss(world.rng)) {
        spawn(world);
        ticks_since_spawn = 0;
    }
}

void munch::munchable_system::spawn(munch::world_system & world) const
{
    namespace com = munch::component;

    auto& entities = world.entities;
    const auto munchable = entities.create();
    entities.emplace<com::munchable>(munchable);
    entities.emplace<com::color>(munchable, 235, 64, 52);

    std::uniform_real_distribution<float> X{ world_bounds.x, world_bounds.x+world_bounds.w };
    std::uniform_real_distribution<float> Y{ world_bounds.y, world_bounds.y+world_bounds.h };
    std::uniform_int_distribution<int> coin_toss{false, true};

    const bool is_on_wall = coin_toss(world.rng);
    const bool is_negative = coin_toss(world.rng);

    const auto& bbox = entities.emplace<com::bbox>(munchable,
        not is_on_wall ? X(world.rng)
                       : (is_negative? world_bounds.x : world_bounds.x+world_bounds.w),
            is_on_wall ? Y(world.rng)
                       : (is_negative? world_bounds.y : world_bounds.y+world_bounds.h),
        20.f
    );

    constexpr float pi = std::numbers::pi_v<float>;
    std::normal_distribution<float> angle_offset{ 0.f, std::sqrt(target_angle_variance) * pi/3 };

    /** angle from center to the spawn position */
    //      |v| sin(theta) = v.y; |v| cos(theta) = v.x
    //      tan(theta) = sin(theta) / cos(theta) -> theta = atan(v.y/v.x)
    const float theta = std::atan2(bbox.y, bbox.x);

    /** angle from spawn position to center with variance */
    //      add pi to reverse the angle
    const float phi = theta + pi + angle_offset(world.rng);
    entities.emplace<com::velocity>(munchable, 100.f*std::cos(phi), 100.f*std::sin(phi));
}

void munch::munchable_system::despawn_any_outside(munch::world_system & world) const
{
    namespace com = munch::component;
    std::vector<entt::entity> to_despawn;

    auto all_munchables = world.entities.view<com::munchable, com::bbox>();
    for (auto &&[entity, bbox] : all_munchables.each()) {
        const SDL_FRect b_rect = bbox;
        if (not SDL_HasIntersectionF(&world_bounds, &b_rect)) {
            to_despawn.push_back(entity);
        }
    }
    world.entities.destroy(to_despawn.begin(), to_despawn.end());
}