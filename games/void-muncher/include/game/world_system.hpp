#pragma once
#include <entt/entity/registry.hpp>
#include "pi/systems/system_graph.hpp"

inline namespace munch {

struct world_system {
    inline static world_system*
    load(pi::system_graph& systems)
    {
        auto& world = systems.emplace<world_system>();
        world.registry = std::make_shared<entt::registry>();
        return &world;
    }

    entt::registry & entities() { return *registry; }
    std::shared_ptr<entt::registry> registry;
};
}