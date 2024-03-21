#pragma once
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <entt/entity/registry.hpp>
#include "pi/systems/system_graph.hpp"

inline namespace munch {

struct world_system {
    inline world_system();
    inline static world_system* load(pi::system_graph& systems);
    inline void init(SDL_Renderer* renderer);

    entt::registry & entities() { return *registry; }
    std::shared_ptr<entt::registry> registry;
    std::mt19937 rng;
    SDL_Point bounds{ 800, 600 };
};
}

munch::world_system::world_system()
    : rng{ std::random_device{}() }
{
}

munch::world_system* munch::world_system::load(pi::system_graph & systems)
{
    auto& world = systems.emplace<world_system>();
    world.registry = std::make_shared<entt::registry>();
    return &world;
}

void munch::world_system::init(SDL_Renderer* renderer)
{
    SDL_GetRendererOutputSize(renderer, &bounds.x, &bounds.y);
}