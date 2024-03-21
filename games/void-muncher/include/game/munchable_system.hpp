#pragma once
#include <memory>
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
    inline void begin() const;

    std::weak_ptr<entt::registry> entity_context;
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

void munch::munchable_system::begin() const
{
    auto entities = entity_context.lock();
    if (not entities) {
        return;
    }
    const auto munchable = entities->create();
    entities->emplace<munch::component::constant_mover>(munchable, 10.f, SDL_FPoint{ 1.f, 0.f });
    entities->emplace<munch::component::color>(munchable, SDL_Color{ 235, 64, 52 });
    entities->emplace<munch::component::size>(munchable, 20.f);
    entities->emplace<munch::component::position>(munchable, SDL_FPoint{ 0.f, 50.f });
}