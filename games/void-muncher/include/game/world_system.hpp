#pragma once
#include <entt/entity/registry.hpp>
#include "pi/systems/system_graph.hpp"

inline namespace munch {

struct world_system {
    entt::registry entities;

    inline static world_system*
    load(pi::system_graph& systems)
    {
        return &systems.emplace<world_system>();
    }
};
}