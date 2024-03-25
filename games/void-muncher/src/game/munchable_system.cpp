#include "game/munchable_system.hpp"
#include <vector>
#include <cstdint>

#include <cmath>
#include <random>
#include <numbers>

#include "pi/system-graph/system_graph.hpp"

#include "basic-components/basic_components.hpp"
#include "game/world_system.hpp"

munch::munchable_system* munch::munchable_system::load(pi::system_graph & systems)
{
    auto* world = systems.load<munch::world_system>();
    if (not world) {
        return nullptr;
    }
    return &systems.emplace<munch::munchable_system>();
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

    std::uniform_int_distribution<std::size_t> random_color{ 0ul, colors.size()-1 };
    const SDL_Color color = colors[random_color(world.rng)];
    entities.emplace<com::color>(munchable, color.r, color.g, color.b);

    std::uniform_real_distribution<float> X{ world.bounds.x, world.bounds.x+world.bounds.w };
    std::uniform_real_distribution<float> Y{ world.bounds.y, world.bounds.y+world.bounds.h };
    std::uniform_int_distribution<int> coin_toss{false, true};

    const bool is_on_wall = coin_toss(world.rng);
    const bool is_negative = coin_toss(world.rng);

    const auto& bbox = entities.emplace<com::bbox>(munchable,
        not is_on_wall ? X(world.rng)
                       : (is_negative? world.bounds.x : world.bounds.x+world.bounds.w),
            is_on_wall ? Y(world.rng)
                       : (is_negative? world.bounds.y : world.bounds.y+world.bounds.h),
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

void munch::munchable_system::despawn_any_outside(munch::world_system & world)
{
    namespace com = munch::component;
    std::vector<entt::entity> to_despawn;

    auto all_munchables = world.entities.view<com::munchable, com::bbox>();
    for (auto &&[entity, bbox] : all_munchables.each()) {
        const SDL_FRect b_rect = bbox;
        if (not SDL_HasIntersectionF(&world.bounds, &b_rect)) {
            to_despawn.push_back(entity);
        }
    }
    world.entities.destroy(to_despawn.begin(), to_despawn.end());
}