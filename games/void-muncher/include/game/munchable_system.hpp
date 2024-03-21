#pragma once
#include <memory>
#include <random>

#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

#include "pi/systems/system_graph.hpp"

#include "mechanics/components.hpp"
#include "movement/constant_mover.hpp"
#include "game/world_system.hpp"

namespace munch {

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
    inline void init(munch::world_system& world_context);
    inline void begin() const;

    std::weak_ptr<entt::registry> entity_context;
    SDL_FRect world_bounds{ 0, 0, 800, 600 };
};
}

munch::munchable_system* munch::munchable_system::load(pi::system_graph & systems)
{
    auto* world = systems.load<munch::world_system>();
    if (not world) {
        return nullptr;
    }
    return &systems.emplace<munch::munchable_system>(world->registry);
}

void munch::munchable_system::init(munch::world_system & world)
{
    world_bounds.x = -static_cast<float>(world.bounds.x)/2;
    world_bounds.y = -static_cast<float>(world.bounds.y)/2;
    world_bounds.w =  static_cast<float>(world.bounds.x);
    world_bounds.h =  static_cast<float>(world.bounds.y);
}

void munch::munchable_system::begin() const
{
    auto entities = entity_context.lock();
    if (not entities) {
        return;
    }
    const auto munchable = entities->create();
    entities->emplace<munch::component::color>(munchable, SDL_Color{ 235, 64, 52 });
    entities->emplace<munch::component::size>(munchable, 20.f);

    std::random_device seed{};
    std::mt19937 rng{seed()};

    std::uniform_real_distribution<float> X{ world_bounds.x, world_bounds.x+world_bounds.w };
    std::uniform_real_distribution<float> Y{ world_bounds.y, world_bounds.y+world_bounds.h };
    std::uniform_int_distribution<int> coin_toss{false, true};

    const bool is_on_wall = coin_toss(rng);
    const bool is_negative = coin_toss(rng);

    const SDL_FPoint position{
        not is_on_wall? X(rng) : (is_negative? world_bounds.x : world_bounds.x+world_bounds.w),
            is_on_wall? Y(rng) : (is_negative? world_bounds.y : world_bounds.y+world_bounds.h)
    };
    entities->emplace<munch::component::position>(munchable, position);

    const float mag = std::sqrt(position.x*position.x + position.y*position.y);
    const SDL_FPoint dir{ -position.x/mag, -position.y/mag };

    entities->emplace<munch::component::constant_mover>(munchable, 100.f, dir);
}